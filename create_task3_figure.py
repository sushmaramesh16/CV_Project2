import cv2
import numpy as np

# Read images
target = cv2.imread('data/olympus/pic.0274.jpg')
match1 = cv2.imread('data/olympus/pic.0273.jpg')
match2 = cv2.imread('data/olympus/pic.1031.jpg')
match3 = cv2.imread('data/olympus/pic.0409.jpg')

# Resize all to same size
size = (280, 280)
target = cv2.resize(target, size)
match1 = cv2.resize(match1, size)
match2 = cv2.resize(match2, size)
match3 = cv2.resize(match3, size)

# Create 2x2 grid
top_row = np.hstack([target, match1])
bottom_row = np.hstack([match2, match3])
grid = np.vstack([top_row, bottom_row])

# Add labels
font = cv2.FONT_HERSHEY_SIMPLEX

# Target
cv2.putText(grid, 'Target: pic.0274.jpg', (10, 30), font, 0.7, (0, 255, 255), 2)

# Match 1
cv2.putText(grid, 'Match 1: pic.0273.jpg', (290, 30), font, 0.7, (0, 255, 255), 2)
cv2.putText(grid, 'Distance: 0.347', (290, 60), font, 0.5, (255, 255, 255), 1)

# Match 2
cv2.putText(grid, 'Match 2: pic.1031.jpg', (10, 310), font, 0.7, (0, 255, 255), 2)
cv2.putText(grid, 'Distance: 0.375', (10, 340), font, 0.5, (255, 255, 255), 1)

# Match 3
cv2.putText(grid, 'Match 3: pic.0409.jpg', (290, 310), font, 0.7, (0, 255, 255), 2)
cv2.putText(grid, 'Distance: 0.380', (290, 340), font, 0.5, (255, 255, 255), 1)

# Add title
cv2.putText(grid, 'Multi-Histogram Matching: Top + Bottom Halves', (50, grid.shape[0] - 10), 
            font, 0.6, (0, 255, 0), 2)

# Save
cv2.imwrite('results/task3_results.jpg', grid)
print("Saved to results/task3_results.jpg")

# Display
cv2.imshow('Task 3 Results - Multi-Histogram', grid)
cv2.waitKey(0)
cv2.destroyAllWindows()