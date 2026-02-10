import cv2
import numpy as np

# Read images
img1 = cv2.imread('data/olympus/pic.1016.jpg')  # Target
img2 = cv2.imread('data/olympus/pic.0986.jpg')  # Match 1
img3 = cv2.imread('data/olympus/pic.0641.jpg')  # Match 2
img4 = cv2.imread('data/olympus/pic.0547.jpg')  # Match 3
img5 = cv2.imread('data/olympus/pic.1013.jpg')  # Match 4

# Resize all to same size
size = (250, 250)
img1 = cv2.resize(img1, size)
img2 = cv2.resize(img2, size)
img3 = cv2.resize(img3, size)
img4 = cv2.resize(img4, size)
img5 = cv2.resize(img5, size)

# Create 2x3 grid
top_row = np.hstack([img1, img2, img3])
bottom_row = np.hstack([img4, img5, np.zeros_like(img5)])

grid = np.vstack([top_row, bottom_row])

# Add labels (FIXED)
font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(grid, 'Target: pic.1016.jpg', (10, 30), font, 0.6, (255, 255, 255), 2)
cv2.putText(grid, 'Match 1: pic.0986.jpg', (260, 30), font, 0.6, (255, 255, 255), 2)
cv2.putText(grid, 'Match 2: pic.0641.jpg', (510, 30), font, 0.6, (255, 255, 255), 2)
cv2.putText(grid, 'Match 3: pic.0547.jpg', (10, 280), font, 0.6, (255, 255, 255), 2)
cv2.putText(grid, 'Match 4: pic.1013.jpg', (260, 280), font, 0.6, (255, 255, 255), 2)

# Save
cv2.imwrite('results/task1_results.jpg', grid)
print("Saved to results/task1_results.jpg")

# Display
cv2.imshow('Task 1 Results', grid)
cv2.waitKey(0)
cv2.destroyAllWindows()