import pandas as pd 
import numpy as np 
from sklearn.model_selection import train_test_split 
from sklearn.preprocessing import StandardScaler 
from sklearn.linear_model import LogisticRegression 
from sklearn.metrics import accuracy_score 
from sklearn.metrics import precision_score 
from sklearn.metrics import recall_score 
from sklearn.metrics import f1_score 
# Load Dataset 
column_names = [ 'Pregnancies', 'Glucose', 'BloodPressure', 'SkinThickness', 'Insulin', 'BMI', 'DiabetesPedigreeFunction', 'Age', 'Outcome' ] 
df = pd.read_csv( "pima-indians-diabetes.data.csv",  names=column_names ) 
# Data Preprocessing 
columns_with_zero = [ 'Glucose', 'BloodPressure', 'SkinThickness', 'Insulin', 'BMI' ] 
for col in columns_with_zero: df[col] = df[col].replace(0, np.nan) 
  # Addressing the FutureWarning by reassigning the column 
df[col] = df[col].fillna(df[col].median()) 
X = df.drop('Outcome', axis=1) 
y = df['Outcome'] 
# Train-Test Split
X_train, X_test, y_train, y_test = train_test_split( X, y, test_size=0.2, random_state=42 ) 
# Logistic Regression without Scaling 
model_no_scaling = LogisticRegression(max_iter=1000) 
model_no_scaling.fit(X_train, y_train) 
y_pred_no_scaling = model_no_scaling.predict(X_test) 
# Evaluation without Scaling 
acc_no = accuracy_score(y_test, y_pred_no_scaling) 
prec_no = precision_score(y_test, y_pred_no_scaling) 
rec_no = recall_score(y_test, y_pred_no_scaling) 
f1_no = f1_score(y_test, y_pred_no_scaling) 
# Feature Scaling 
scaler = StandardScaler() 
X_train_scaled = scaler.fit_transform(X_train) 
X_test_scaled = scaler.transform(X_test) 
# Logistic Regression with Scaling 
model_scaling = LogisticRegression(max_iter=1000) 
model_scaling.fit(X_train_scaled, y_train) 
y_pred_scaling = model_scaling.predict(X_test_scaled) 
# Evaluation with Scaling 
acc_scale = accuracy_score(y_test, y_pred_scaling) 
prec_scale = precision_score(y_test, y_pred_scaling) 
rec_scale = recall_score(y_test, y_pred_scaling) 
f1_scale = f1_score(y_test, y_pred_scaling) 
# Display Results 
print("WITHOUT FEATURE SCALING") 
print("Accuracy :", acc_no) 
print("Precision:", prec_no) 
print("Recall   :", rec_no) 
print("F1 Score :", f1_no) 
print("\nWITH FEATURE SCALING") 
print("Accuracy :", acc_scale) 
print("Precision:", prec_scale) 
print("Recall   :", rec_scale) 
print("F1 Score :", f1_scale) 
# Comparison Table 
results = pd.DataFrame({ 'Metric':['Accuracy', 'Precision', 'Recall', 'F1 Score'],
                        'Without Scaling':[acc_no, prec_no, rec_no, f1_no], 
                        'With Scaling':[acc_scale, prec_scale, rec_scale, f1_scale] }) 
print("\nComparison of Models") 
print(results)
