#!/usr/bin/env python

"""SPS nonlinear calibration and evaluation

The adc data might be non-linear, so this file will attempt to fit a non-linear model to the data.

Stephen Taylor 5/20/2024
"""

#%%
import os
import pandas as pd
import numpy as np
from numpy.polynomial import Polynomial
import matplotlib.pyplot as plt
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score, mean_absolute_percentage_error
from typing import List, Dict
from scipy.stats import norm

#%%
### Load the data ###
def load_data(datafiles):
    df_list = []
    for d in datafiles:
        df = pd.read_csv(d)
        df_list.append(df)
    
    data = pd.concat(df_list, ignore_index=True)
    data["V_in"] = data["V_in"]
    data["I_in"] = data["I_in"] 
    data["I_meas"] = data["I_sps"] 
    data["V_meas"] = data["V_sps"]
    
    return data

#%%
### Load the calibration CSVs ###
datafiles = ["data/calibration_data/sps4_voltage_calib_-3to3v.csv"] # load voltage

data = load_data(datafiles)

#%%
#### Plot the SMU voltage and the raw SPS values to check for linearity ###
plt.figure()
plt.scatter(data["V_in"], data["V_sps"], s=3)
plt.title("Data visualization")
plt.xlabel("Input (V)")
plt.ylabel("ADC Raw")
plt.show()

plt.figure()
plt.hist(data["V_sps"] - data["V_in"], bins=30, edgecolor='k', alpha=0.7)
plt.title("Histogram of Residuals (Pre Calibration)")
plt.xlabel("Residuals")
plt.ylabel("Frequency")
plt.show()

#%%
### Fit the polynomial model (non-segmented) ###
model, params = Polynomial.fit(data["V_meas"], data["V_in"], 4, full=True)
print(model)
print(model.convert().coef)

#%%
### Load the eval files ###
evalfiles = ["data/eval_data/sps4_voltage_eval_-3to3v_sweep1.csv"]
eval_data = load_data(evalfiles)

#%%


#%% Histogram plotting function
def plot_residual_histogram(residuals, title):
    """Plot a histogram of residuals
    
    Args:
        residuals: Iterable of residuals values
        title: Title of the plot
    """
    mu, std = norm.fit(residuals)
    normdist_x = np.linspace(mu - 3*std, mu + 3*std, 100)
    normdist_y = norm.pdf(normdist_x, mu, std)
    
    plt.figure()
    plt.hist(residuals, bins=30, density=True, edgecolor='k', alpha=0.7)
    #plt.plot(normdist_x, normdist_y, color="r")
    plt.title(title)
    plt.xlabel("Residuals")
    plt.ylabel("Frequency")
    plt.show()



#%% Segment the data

def segment_data(df: pd.DataFrame, column: str, thresholds: List[float]) -> Dict[str, pd.DataFrame]:
    segments = {}
    thresholds = sorted(thresholds)  # Ensure thresholds are sorted

    # Create segments for each threshold range
    for i in range(len(thresholds) - 1):
        lower_threshold = thresholds[i]
        upper_threshold = thresholds[i + 1]
        segment_name = f"between_{lower_threshold}_and_{upper_threshold}"
        segments[segment_name] = df[(df[column] > lower_threshold) & (df[column] <= upper_threshold)]

    # Optionally add the last segment if the upper bound is desired
    # last_segment_name = f"greater_than_{thresholds[-1]}"
    # segments[last_segment_name] = df[df[column] > thresholds[-1]]

    return segments

#%% Segment the data using the function

thresholds = [-3.3, -1.65, 0, 1.3, 3.3]
segmented_data = segment_data(data, "V_in", thresholds)
segmented_eval_data = segment_data(eval_data, "V_in", thresholds)

#%% Print the segmented DataFrames
for segment_name, segment_df in segmented_data.items():
    print(f"\n{segment_name}:")
    print(segment_df)
    print(f"Shape: {segment_df.shape}")

#%% Fit the polynomial model (segmented) ###
def fit_segmented_models(segments: Dict[str, pd.DataFrame]) -> Dict[str, List[float]]:
    segmented_models = {}

    for segment_name, segment_df in segments.items():
        if segment_df["V_meas"].empty or segment_df["V_in"].empty:
            print(f"Warning: One of the columns in {segment_name} is empty.")

        model_name = f"{segment_name}_model"
        model = Polynomial.fit(segment_df["V_meas"], segment_df["V_in"], 3)
        segmented_models[model_name] = model
    
    return segmented_models


#%%
segmented_models = fit_segmented_models(segmented_data)

def print_models_and_coefficients(models: Dict[str, Polynomial]):
    for model_name, model in models.items():
        print(f"{model_name}")
        print(model)
        print()  # Blank line for better readability

print_models_and_coefficients(segmented_models)

#%% Test the fit 
def predict_using_segmented_models(models: Dict[str, Polynomial], eval_data_segments: Dict[str, pd.DataFrame]) -> pd.DataFrame:
    all_segmented_predictions = []

    for model_name, model in models.items():
        # Extract the segment name to match with evaluation data
        segment_name = model_name.replace("_model", "")  # Assuming naming convention is consistent

        if segment_name in eval_data_segments:
            segment_df = eval_data_segments[segment_name]
            if not segment_df.empty:
                # Assuming you want to predict using 'V_meas' column
                predictions = model(segment_df["V_meas"])  # Make predictions
                # Create a DataFrame to store predictions
                predictions_df = segment_df.copy()
                predictions_df["Predictions"] = predictions
                predictions_df["Model_Name"] = model_name
                all_segmented_predictions.append(predictions_df)
        else:
            print(f"Warning: No evaluation segment found for model '{model_name}'")

    # Concatenate all the DataFrames into one DataFrame
    total_predictions_df = pd.concat(all_segmented_predictions, ignore_index=True)
    return total_predictions_df

segmented_predictions_df = predict_using_segmented_models(segmented_models, segmented_eval_data)
#%%
### Test the fit ###
predicted = model(eval_data["V_meas"])
residuals = eval_data["V_in"] - predicted

print("Evaluate non-segmented model using sklearn.metrics")
mae = mean_absolute_error(eval_data["V_in"], predicted)
rmse = np.sqrt(mean_squared_error(eval_data["V_in"], predicted))
r2 = r2_score(eval_data["V_in"], predicted)
mape = mean_absolute_percentage_error(eval_data["V_in"], predicted)
print(f"Mean absolute error: {mae:.4f}")
print(f"Mean absolute percentage error: {mape:.4f}")
print(f"Root mean square error: {rmse:.4f}")
print(f"R-squared: {r2:.4f}")

plt.figure()
plt.title("SPS to predicted voltage")
plt.scatter(eval_data["V_sps"], predicted, label="Non-linear model")
plt.ylabel("Predicted (V)")
plt.xlabel("Raw SPS value")
plt.legend()
plt.show()

plt.figure()
plt.title("Residual plot")
plt.scatter(predicted, residuals)
plt.axhline(y=0, color='r', linestyle='--')
plt.ylabel("Residuals")
plt.xlabel("Predicted (V)")
plt.legend()
plt.show()
plot_residual_histogram(residuals, "Histogram of Residuals for Non-segmented model")

segmented_residuals = eval_data["V_in"] - segmented_predictions_df

print("Evaluate segmented model using sklearn.metrics")
mae = mean_absolute_error(eval_data["V_in"], segmented_predictions_df)
rmse = np.sqrt(mean_squared_error(eval_data["V_in"], segmented_predictions_df))
r2 = r2_score(eval_data["V_in"], segmented_predictions_df)
mape = mean_absolute_percentage_error(eval_data["V_in"], segmented_predictions_df)
print(f"Mean absolute error: {mae:.4f}")
print(f"Mean absolute percentage error: {mape:.4f}")
print(f"Root mean square error: {rmse:.4f}")
print(f"R-squared: {r2:.4f}")

plt.figure()
plt.title("SPS to predicted voltage")
plt.scatter(eval_data["V_sps"], segmented_predictions_df, label="Non-linear model")
plt.ylabel("Predicted (V)")
plt.xlabel("Raw SPS value")
plt.legend()
plt.show()

plt.figure()
plt.title("Residual plot")
plt.scatter(segmented_predictions_df, segmented_residuals)
plt.axhline(y=0, color='r', linestyle='--')
plt.ylabel("Residuals")
plt.xlabel("Predicted (V)")
plt.legend()
plt.show()
plot_residual_histogram(segmented_residuals, "Histogram of Residuals for Non-segmented model")
#%%