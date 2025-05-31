import numpy as np
import matplotlib.pyplot as plt

# Original input values
# x = np.array([0, 23, 28, 35, 39, 46, 51, 59, 63, 71, 76, 81, 88, 93, 99, 104, 111, 117, 122, 127, 135, 140, 147, 152, 158, 165, 172, 176, 184, 188, 194, 199, 207, 212, 220, 224, 231, 237, 242, 248, 255, 261, 264, 271, 280, 284, 289, 295, 302, 307, 313, 319, 326, 332, 336, 344, 352, 355, 359, 368, 374, 380, 384, 391, 398, 403])
# y = np.array([0, 1.01, 1.28, 1.55, 1.72, 2.04, 2.28, 2.56, 2.75, 3.06, 3.24, 3.51, 3.79, 3.99, 4.24, 4.53, 4.74, 5.00, 5.23, 5.54, 5.74, 5.97, 6.26, 6.51, 6.72, 7.02, 7.29, 7.49, 7.76, 7.99, 8.26, 8.50, 8.78, 9.02, 9.28, 9.53, 9.76, 10.05, 10.28, 10.51, 10.77, 11.03, 11.26, 11.50, 11.77, 12.03, 12.27, 12.51, 12.76, 13.00, 13.26, 13.51, 13.75, 14.02, 14.25, 14.51, 14.78, 15.00, 15.25, 15.50, 15.76, 16.03, 16.24, 16.51, 16.77, 17.03])

x = np.array([0, 16,48,83,167,252,334,419,531,590,671,755,839,924,967])
y = np.array([0, 1.1,3.006,5.049,10.01,15.04,19.90,24.96,31.62,35.12,40.08,45.07,50.10,55.08,57.73])

# Fit a polynomial of degree n
degree = 1
coefficients = np.polyfit(x, y, degree)
polynomial = np.poly1d(coefficients)

print("Calculated coefficients for a polynomial fit of  " + str(degree) + " degree")
print(coefficients)
print(polynomial)

# Evaluate the polynomial
fitted_y = polynomial(x)

# Adjust values based on gains
# x_adjusted = x
# y_adjusted = y

# Fit a polynomial to the adjusted values
# coefficients_adjusted = np.polyfit(x_adjusted, y_adjusted, degree)
# polynomial_adjusted = np.poly1d(coefficients_adjusted)

# Plotting
plt.figure(figsize=(12, 6))

# Original plot
plt.subplot(2,1, 1)
plt.scatter(x, y, color='blue', label='Original Data')
x_range = np.linspace(min(x), max(x), 500)
plt.plot(x_range, polynomial(x_range), color='red', label=f'Fitted Polynomial (degree {degree})')
# plt.plot(x, x*0.042, color='pink', label='xes')
plt.xlabel('ADC 10 bit values')
plt.ylabel('Voltave [V]')
plt.legend()
plt.title('Polynomial Fit (Original Data)')
plt.xticks(np.arange(0, max(x), 50))  
plt.yticks(np.arange(0, max(y), max(y)/10))

# Adjusted plot
# plt.subplot(2,1,2)
# plt.scatter(x_adjusted, y_adjusted, color='green', label='Adjusted Data')
# x_range_adjusted = np.linspace(min(x_adjusted), max(x_adjusted), 500)
# plt.plot(x_range_adjusted, polynomial_adjusted(x_range_adjusted), color='purple', label=f'Fitted Polynomial (degree {degree})')
# plt.xlabel('ADC 10 bit values')
# plt.ylabel('Voltave [V]')
# plt.legend()
# plt.title('Polynomial Fit (Adjusted Data)')
# plt.xticks(np.arange(0, max(x_adjusted), 50))  
# plt.yticks(np.arange(0, max(y_adjusted), max(y_adjusted)/10))


# Show plots
plt.tight_layout()
plt.show()


# VSCALE_FACTOR = 1e4
# a= -1.12162388e-06 * VSCALE_FACTOR * 1e5
# b= 4.25139124e-02 * VSCALE_FACTOR
# c= 9.74947993e-02 * VSCALE_FACTOR
# 
# x_max = 1024 
# x = x_max
# 
# result1 = (a * x / 1e5) * x / 1e4
# result2 = result1 / 1e4
# result3 = result2 + (b * x)/1e4 + c/1e4
# 
# print("Result 1: " + str(result1)) 
# print("Result 2: " + str(result2)) 
# print("Result 3: " + str(result3)) 