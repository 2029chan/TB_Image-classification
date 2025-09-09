%tensorflow_version 2.x

import tensorflow as tf
import numpy as np
import cv2
import matplotlib.pyplot as plt

from google.colab import drive
drive.mount('/content/drive')

model_path = '/content/drive/My Drive/kaggle/TB_Reduced.h5'
from tensorflow.keras.models import load_model

model = load_model(model_path)

def preprocess_image(image_path, target_size=(96, 96)):
    """
    Preprocess the image for prediction.
    Args:
        image_path (str): Path to the input image.
        target_size (tuple): Target size for the image as expected by the model.
    Returns:
        numpy.ndarray: Preprocessed image.
    """
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)  # Load as grayscale
    img = cv2.resize(img, target_size)  # Resize to the target size
    img = img / 255.0  # Normalize the image
    img = np.expand_dims(img, axis=-1)  # Add channel dimension for grayscale (H, W, 1)
    img = np.expand_dims(img, axis=0)  # Add batch dimension (1, H, W, C)
    return img

# Predict and display results
def predict_image(image_path):
  """
    Predict and display results for a given image.
    Args:
        image_path (str): Path to the image.
    """
  preprocessed_img = preprocess_image(image_path) # Fixed: Removed extra indentation
  prediction = model.predict(preprocessed_img)
  predicted_class = np.argmax(prediction)
  class_labels = ['Normal', 'Tuberculosis']  # Update as per your classes
  confidence = prediction[0][predicted_class] * 100  # Get confidence level

  # Display the image and prediction
  img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
  plt.imshow(img, cmap='gray')
  plt.axis('off')
  plt.title(f"Prediction: {class_labels[predicted_class]} ({confidence:.2f}%)")
  plt.show()

# Test with a sample image
image_path = "/content/drive/My Drive/kaggle/Tuberculosis-662.png"  # Replace with the path to your test image
predict_image(image_path)

# ... other code ...

# Use this modified export code instead of your current version
# Use this exact conversion script
def convert_weights(weights, name):
    with open(f"{name}.txt", "w") as f:
        if len(weights.shape) == 4:  # Conv2D weights
            weights = weights.reshape(-1)
        np.savetxt(f, weights, delimiter=',', fmt='%0.9f')

# In your Python code after model.load_model():
weights = model.get_weights()
convert_weights(weights[0], "hidden_weights")  # Conv2D weights
convert_weights(weights[1], "hidden_biases")   # Conv2D biases
convert_weights(weights[2], "output_weights") # Output layer weights
convert_weights(weights[3], "output_biases")  # Output layer biases



