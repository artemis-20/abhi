import numpy as np 
import pandas as pd 
from sklearn.datasets import load_diabetes 
from sklearn.model_selection import train_test_split 
from sklearn.preprocessing import StandardScaler 
from sklearn.linear_model import LinearRegression 
from sklearn.linear_model import Ridge, Lasso 
from sklearn.model_selection import GridSearchCV 
from sklearn.metrics import mean_squared_error, r2_score
# Load Dataset 
diabetes = load_diabetes() 
X = diabetes.data 
y = diabetes.target 
# Train-Test Split
X_train, X_test, y_train, y_test = train_test_split( X, y, test_size=0.2, random_state=42) 
# Feature Scaling 
scaler = StandardScaler() 
X_train_scaled = scaler.fit_transform(X_train) 
X_test_scaled = scaler.transform(X_test) 
# Linear Regression 
lr = LinearRegression() 
lr.fit(X_train_scaled, y_train) 
y_pred_lr = lr.predict(X_test_scaled) 
mse_lr = mean_squared_error(y_test, y_pred_lr) 
r2_lr = r2_score(y_test, y_pred_lr) 
# Ridge Regression with Cross Validation 
ridge = Ridge() 
ridge_params = { 'alpha': [0.001, 0.01, 0.1, 1, 10, 100]} 
ridge_cv = GridSearchCV( ridge, ridge_params, cv=5, scoring='r2' ) 
ridge_cv.fit(X_train_scaled, y_train) 
best_ridge = ridge_cv.best_estimator_
y_pred_ridge = best_ridge.predict(X_test_scaled) 
mse_ridge = mean_squared_error(y_test, y_pred_ridge) 
r2_ridge = r2_score(y_test, y_pred_ridge) 
# Lasso Regression with Cross Validation 
lasso = Lasso(max_iter=10000) 
lasso_params = { 'alpha': [0.001, 0.01, 0.1, 1, 10] } 
lasso_cv = GridSearchCV( lasso, lasso_params, cv=5, scoring='r2' ) 
lasso_cv.fit(X_train_scaled, y_train) 
best_lasso = lasso_cv.best_estimator_ 
y_pred_lasso = best_lasso.predict(X_test_scaled) 
mse_lasso = mean_squared_error(y_test, y_pred_lasso) 
r2_lasso = r2_score(y_test, y_pred_lasso) 
# Results 
print("Linear Regression") 
print("MSE =", mse_lr) 
print("R2 Score =", r2_lr) 
print("\nBest Ridge Alpha =", ridge_cv.best_params_['alpha'])
print("Ridge Regression") 
print("MSE =", mse_ridge) 
print("R2 Score =", r2_ridge) 
print("\nBest Lasso Alpha =", lasso_cv.best_params_['alpha']) 
print("Lasso Regression") 
print("MSE =", mse_lasso) 
print("R2 Score =", r2_lasso) 
# Comparison Table 
results = pd.DataFrame({ 'Model': ['Linear Regression', 'Ridge Regression', 'Lasso Regression'],
                        'MSE': [mse_lr, mse_ridge, mse_lasso], 
                        'R2 Score': [r2_lr, r2_ridge, r2_lasso] }) 
print("\nComparison of Models") 
print(results)
