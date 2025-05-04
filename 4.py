# parallel_linear_regression.py

from mpi4py import MPI
import numpy as np

# Initialize MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Generate synthetic data (same on all ranks for simplicity)
np.random.seed(42)
X = np.random.rand(100, 1)
y = 2 * X + 1 + 0.1 * np.random.randn(100, 1)  # Corrected data size

# Split data among processes
split_size = len(X) // size
start = rank * split_size
end = (rank + 1) * split_size
X_part = X[start:end]
y_part = y[start:end]

# Initialize weights
w = np.random.randn(1)
b = np.random.randn(1)

# Training parameters
learning_rate = 0.1
epochs = 50

for epoch in range(epochs):
    # Forward pass
    y_pred = X_part.dot(w) + b
    error = y_pred - y_part

    # Compute gradients
    grad_w_local = 2 * np.mean(error * X_part)
    grad_b_local = 2 * np.mean(error)

    # Average gradients across all processes
    total_grad_w = comm.allreduce(grad_w_local, op=MPI.SUM) / size
    total_grad_b = comm.allreduce(grad_b_local, op=MPI.SUM) / size

    # Update weights
    w = w - learning_rate * total_grad_w
    b = b - learning_rate * total_grad_b

    if rank == 0 and epoch % 10 == 0:
        print(f"Epoch {epoch}, w = {w[0]:.4f}, b = {b[0]:.4f}")

# Final weights from rank 0
if rank == 0:
    print(f"Final model: y = {w[0]:.4f} * x + {b[0]:.4f}")