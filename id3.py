import pandas as pd
import numpy as np 
import matplotlib.pyplot as plt 
from sklearn.model_selection import train_test_split 
from sklearn.preprocessing import LabelEncoder 
from sklearn.tree import DecisionTreeClassifier 
from sklearn.tree import plot_tree 
from sklearn.metrics import accuracy_score 
# Load Dataset
df = pd.read_excel("Online Retail.xlsx")
# Remove Missing Values 
df = df.dropna()
# Create Total Amount 
df["TotalAmount"] = df["Quantity"] * df["UnitPrice"]
# Customer Aggregation 
customer_data = df.groupby("CustomerID").agg({
  "Quantity":"sum",
  "TotalAmount":"sum",
  "InvoiceNo":"nunique" }).reset_index() 
customer_data.columns = [ "CustomerID", "TotalQuantity", "TotalSpending", "PurchaseFrequency" ] 
# Customer Segmentation 
customer_data["Segment"] = pd.qcut( customer_data["TotalSpending"], q=3, labels=["Low","Medium","High"] ) 
# Features
X = customer_data[ ["TotalQuantity", "TotalSpending", "PurchaseFrequency"] ]
# Target 
y = customer_data["Segment"]
# Encode Target
encoder = LabelEncoder()
y = encoder.fit_transform(y) 
# Train Test Split 
X_train, X_test, y_train, y_test = train_test_split( X, y, test_size=0.2, random_state=42 )
# ID3 Decision Tree 
model = DecisionTreeClassifier( criterion="entropy", random_state=42 )
model.fit(X_train, y_train) 
# Prediction 
y_pred = model.predict(X_test) 
# Accuracy 
accuracy = accuracy_score(y_test, y_pred) 
print("Accuracy:", accuracy) 
# Feature Importance 
importance = pd.DataFrame({ "Feature": X.columns, "Importance": model.feature_importances_ }) 
print("\nFeature Importance") 
print(importance) 
# Tree Visualization 
plt.figure(figsize=(16,8)) 
plot_tree( model, feature_names=X.columns, class_names=encoder.classes_, filled=True ) 
plt.show()
