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
plt.scatter(eval_data["V_sps"], predicted, label = "Non-linear model")
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
def plot_residual_histogram(res, ax=None):
    """Plot a histogram of residuals on a set of axis
    
    Args:
        res: Iterable of residuals values
        ax: Axis to plot on. If "None", a new figure is created
        
    Returns:
        Mean and standard deviation of the normal distribution
    """
    
    if ax is None:
        pass
   
    mu, std = norm.fit(res)
    print(f"Norm distribution parameters: mu = {mu:.4f}, std = {std:.4f}")

    normdist_x = np.linspace(mu - 3*std, mu + 3*std, 100)
    normdist_y = norm.pdf(normdist_x, mu, std)
    
    return mu, std
#%%

mu, std = norm.fit(residuals)
print(f"Norm distribution parameters: mu = {mu:.4f}, std = {std:.4f}")

normdist_x = np.linspace(mu - 3*std, mu + 3*std, 100)
normdist_y = norm.pdf(normdist_x, mu, std)

plt.figure()
plt.hist(residuals, bins=30, density=True, edgecolor='k', alpha=0.7)
plt.plot(normdist_x, normdist_y, color="r")
plt.title("Histogram of Residuals (Post Calibration)")
plt.xlabel("Residuals")
plt.ylabel("Frequency")
plt.show()

std_dev = np.std(residuals)
mean = np.mean(residuals)
print(f"Residual mean: {mean:.4f}, std: {std_dev:.4f}")

within_one_std_dev = np.sum(np.abs(residuals) <= std_dev) / len(residuals) * 100
print(f"Percentage of residuals within one standard deviation: {within_one_std_dev:.2f}%")

for index, value in enumerate(residuals):
    if np.abs(value) > 100:
        print(f"Index: {index}, Value: {value}")

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
### Fit and evaluate segmented models ###
def fit_and_evaluate_segmented_models(segmented_data, eval_data):
    metrics_list = []
    for i, seg in enumerate(segmented_data):
        if not seg.empty:
            print(f"Fitting model for segment {i+1} with {len(seg)} samples")  # Debugging statement
            model, _ = Polynomial.fit(seg["V_meas"], seg["V_in"], 3, full=True)
            predicted = model(eval_data["V_meas"])
            residuals = eval_data["V_in"] - predicted
            mae = mean_absolute_error(eval_data["V_in"], predicted)
            rmse = np.sqrt(mean_squared_error(eval_data["V_in"], predicted))
            r2 = r2_score(eval_data["V_in"], predicted)
            mape = mean_absolute_percentage_error(eval_data["V_in"], predicted)
            metrics_list.append((mae, rmse, r2, mape, residuals))
    return metrics_list

segmented_metrics_1 = fit_and_evaluate_segmented_models(segmented_data_1, eval_data)
segmented_metrics_2 = fit_and_evaluate_segmented_models(segmented_data_2, eval_data)

#%%
### Print segmented model performance ###
print("Segmented model scheme 1 performance:")
for i, metrics in enumerate(segmented_metrics_1):
    if metrics:
        print(f"Segment {i+1}: MAE: {metrics[0]:.4f}, RMSE: {metrics[1]:.4f}, R2: {metrics[2]:.4f}, MAPE: {metrics[3]:.4f}")
    else:
        print(f"Segment {i+1}: No data")

print("Segmented model scheme 2 performance:")
for i, metrics in enumerate(segmented_metrics_2):
    if metrics:
        print(f"Segment {i+1}: MAE: {metrics[0]:.4f}, RMSE: {metrics[1]:.4f}, R2: {metrics[2]:.4f}, MAPE: {metrics[3]:.4f}")
    else:
        print(f"Segment {i+1}: No data")

#%%
### Plot residual histograms for segmented models ###
for i, metrics in enumerate(segmented_metrics_1):
    if metrics:
        mu, std = plot_residual_histogram(metrics[4])
        plt.hist(residuals, bins=30, density=True, edgecolor='k', alpha=0.7)
        plt.plot(normdist_x, normdist_y, color="r")
        plt.title(f"Histogram of Residuals (Segmented Model Scheme 1 - Segment {i+1})")
        plt.xlabel("Residuals")
        plt.ylabel("Frequency")
        plt.show()

for i, metrics in enumerate(segmented_metrics_2):
    if metrics:
        mu, std = plot_residual_histogram(metrics[4])
        plt.hist(residuals, bins=30, density=True, edgecolor='k', alpha=0.7)
        plt.plot(normdist_x, normdist_y, color="r")
        plt.title(f"Histogram of Residuals (Segmented Model Scheme 2 - Segment {i+1})")
        plt.xlabel("Residuals")
        plt.ylabel("Frequency")
        plt.show()
#%%