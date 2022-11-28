"""File to parse a new dataset and output it in a format that aligns with neural network

Optionally creates a weight file with weights initialized from an Xavier distribution
"""

# %%
import pandas as pd
import numpy as np 
from sklearn.model_selection import train_test_split


# %% Read and parse input files
TRAIN_FILE = "./samples/SAheart.txt"
TEST_FILE = "./samples/SAheart_test.txt"
INIT_FILE = "./samples/SAheart_init.txt"

df = pd.read_csv("./samples/SAheart.csv").drop(["row.names", "famhist"], axis=1)
train, test = train_test_split(df, test_size=0.2, random_state=31415)
np.savetxt(TRAIN_FILE, train.values, fmt='%0.3f')
np.savetxt(TEST_FILE, test.values, fmt='%0.3f')


# %% Add initial line with number of examples, number if inputs, and number of outputs
num_inputs = df.columns.values.shape[0] - 1
num_outputs = 1
num_hidden = 41

with open(TRAIN_FILE, 'r+') as f:
    content = f.read()
    f.seek(0, 0)
    f.write(f"{train.shape[0]} {num_inputs} {num_outputs}\n{content}")

with open(TEST_FILE, 'r+') as f:
    content = f.read()
    f.seek(0, 0)
    f.write(f"{test.shape[0]} {num_inputs} {num_outputs}\n{content}")


# %% Generate initial weights

def xavier_init(input_dim, output_dim, rng):
    """Initializes weight vector given in/out dimensions
    
    Obtained from TensorFlow Documentation
    """
    xavier_lim = np.sqrt(6.)/np.sqrt(input_dim + output_dim)
    weight_vals = rng.uniform(size=(input_dim, output_dim), low=-xavier_lim, high=xavier_lim)
    return weight_vals

np_rng = np.random.default_rng(31415)
w = xavier_init(num_inputs + 1, num_hidden, np_rng)
np.savetxt(INIT_FILE, w.T, fmt='%0.3f')
output = xavier_init(num_hidden + 1, num_outputs, np_rng)

with open(INIT_FILE, 'r+') as f:
    content = f.read()
    f.seek(0, 0)
    f.write(f"{num_inputs} {num_hidden} {num_outputs}\n{content}")

with open(INIT_FILE, 'a') as f:
    np.savetxt(f, output.T, fmt='%0.3f')   

