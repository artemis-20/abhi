# Import Libraries
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import StandardScaler
#Loading Dataset
housing = fetch_california_housing(as_frame=True)
df = housing.frame
print(df.head())
#Select One feature
X = df[['AveRooms']].values
y = df['MedHouseVal'].values
#Train test split
X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.2,
    random_state=42
)
#feature scaling
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
#Initialize parameters
m = len(y_train)
theta0 = 0
theta1 = 0
learning_rate = 0.01
iterations = 1000
#Gradient Descent
for i in range(iterations):
    predictions = theta0 + theta1 * X_train_scaled.flatten()
    error = predictions - y_train
    d_theta0 = (1/m) * np.sum(error)
    d_theta1 = (1/m) * np.sum(error * X_train_scaled.flatten())
    theta0 = theta0 - learning_rate * d_theta0
    theta1 = theta1 - learning_rate * d_theta1
#Prediction
y_pred_gd = theta0 + theta1 * X_test_scaled.flatten()
#Evaluation
mse_gd = mean_squared_error(y_test, y_pred_gd)
r2_gd = r2_score(y_test, y_pred_gd)
print("Gradient Descent")
print(y_pred_gd[:10])
print("Theta0 =", theta0)
print("Theta1 =", theta1)
print("MSE =", mse_gd)
print("R2 Score =", r2_gd)
#Add Bias term
X_train_ne = np.c_[np.ones((len(X_train),1)), X_train]
X_test_ne = np.c_[np.ones((len(X_test),1)), X_test]
theta=np.linalg.inv(X_train_ne.T.dot(X_train_ne)).dot(X_train_ne.T).dot(y_train)
print(theta)
# Prediction using Normal Equation
y_pred_ne = X_test_ne.dot(theta)
mse_ne = mean_squared_error(y_test, y_pred_ne)
r2_ne = r2_score(y_test, y_pred_ne)
print(y_pred_ne[:10])
print("Normal Equation")
print("Theta0 =", theta[0])
print("Theta1 =", theta[1])
print("MSE =", mse_ne)
print("R2 Score =", r2_ne)
plt.figure(figsize=(8,6))
plt.scatter(
    X_test,
    y_test,
    color='blue',
    alpha=0.5,
    label='Actual Data'
)
sorted_index = X_test.flatten().argsort()
plt.plot(
    X_test.flatten()[sorted_index],
    y_pred_ne[sorted_index],
    color='red',
    linewidth=3,
    label='Regression Line'
)
plt.xlabel("Average Rooms")
plt.ylabel("Median House Value")
plt.title("Linear Regression on California Housing")
plt.legend()
plt.show()
