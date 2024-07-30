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
### Test the fit ###
predicted = model(eval_data["V_meas"])
residuals = eval_data["V_in"] - predicted

print("Evaluate using sklearn.metrics")
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

plt.figure()
plt.title("Histogram of Residuals (Post Calibration)")
plt.hist(residuals, bins=30, edgecolor='black')
plt.xlabel("Residuals")
plt.ylabel("Frequency")
plt.show()

residual_average = np.average(residuals)
print("Average residual: ", residual_average)

#%%
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
    plt.plot(normdist_x, normdist_y, color="r")
    plt.title(title)
    plt.xlabel("Residuals")
    plt.ylabel("Frequency")
    plt.show()

#%%
### Segment the data ###
segments_scheme_1 = [(-3, -0.5), (-0.5, 0.5), (0.5, 3)]
segments_scheme_2 = [(-3, -2), (-2, 0), (0, 2), (2, 3)]

def segment_data(data, segments):
    segmented_data = []
    for (start, end) in segments:
        seg = data[(data["V_in"] >= start) & (data["V_in"] < end)]
        print(f"Segment {start} to {end} contains {len(seg)} samples")  # Debugging statement
        if not seg.empty:
            segmented_data.append(seg)
    return segmented_data

segmented_data_1 = segment_data(data, segments_scheme_1)
segmented_data_2 = segment_data(data, segments_scheme_2)

#%%
### Aggregate residuals from all segments ###
def fit_and_aggregate_residuals(segmented_data, eval_data, poly_degree=4):
    all_residuals = []
    total_predicted = np.zeros(len(eval_data))

    for seg in segmented_data:
        if not seg.empty:
            model, _ = Polynomial.fit(seg["V_meas"], seg["V_in"], poly_degree, full=True)
            predicted = model(eval_data["V_meas"])
            residuals = eval_data["V_in"] - predicted
            all_residuals.extend(residuals)

            # Aggregate the predicted values
            total_predicted += predicted

    # Average the predicted values if there are overlaps
    total_predicted /= len(segmented_data)

    return all_residuals, total_predicted

aggregated_residuals_1, total_predicted_1 = fit_and_aggregate_residuals(segmented_data_1, eval_data)
aggregated_residuals_2, total_predicted_2 = fit_and_aggregate_residuals(segmented_data_2, eval_data)

#%%
### Print aggregated residual histograms ###
plot_residual_histogram(aggregated_residuals_1, "Histogram of Residuals for Segmented Model Scheme 1")
plot_residual_histogram(aggregated_residuals_2, "Histogram of Residuals for Segmented Model Scheme 2")

#%%
### Plot combined residuals and predicted values for segmented models ###
def plot_combined_residuals_and_predictions(eval_data, total_predicted, title):
    residuals = eval_data["V_in"] - total_predicted

    plt.figure()
    plt.title(f"{title} - Predicted vs Measured")
    plt.scatter(eval_data["V_meas"], total_predicted, label="Segmented model")
    plt.ylabel("Predicted (V)")
    plt.xlabel("Measured (V)")
    plt.legend()
    plt.show()
    
    plt.figure()
    plt.title(f"{title} - Residual plot")
    plt.scatter(total_predicted, residuals)
    plt.axhline(y=0, color='r', linestyle='--')
    plt.ylabel("Residuals")
    plt.xlabel("Predicted (V)")
    plt.legend()
    plt.show()

plot_combined_residuals_and_predictions(eval_data, total_predicted_1, "Segmented Model Scheme 1")
plot_combined_residuals_and_predictions(eval_data, total_predicted_2, "Segmented Model Scheme 2")

#%%
### Summarize metrics for comparison ###
def summarize_metrics(non_segmented_metrics, segmented_metrics_list, scheme_labels):
    summary = []
    
    # Non-segmented model metrics
    summary.append(["Non-Segmented"] + list(non_segmented_metrics))
    
    for i, metrics_list in enumerate(segmented_metrics_list):
        for j, metrics in enumerate(metrics_list):
            summary.append([f"{scheme_labels[i]} - Segment {j+1}"] + list(metrics))
    
    return pd.DataFrame(summary, columns=["Model", "MAE", "RMSE", "R2", "MAPE"])

# Non-segmented model metrics
non_segmented_metrics = (mae, rmse, r2, mape)

# Segmented metrics list
segmented_metrics_list = [segmented_metrics_1, segmented_metrics_2]

# Scheme labels
scheme_labels = ["Scheme 1", "Scheme 2"]

# Summarize metrics
metrics_summary = summarize_metrics(non_segmented_metrics, segmented_metrics_list, scheme_labels)
print(metrics_summary)
#%%