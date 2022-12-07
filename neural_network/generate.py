"""File to parse a new dataset and output it in a format that aligns with neural network

Optionally creates a weight file with weights initialized from an Xavier distribution
"""

# %%
import pandas as pd
import numpy as np 
from sklearn.model_selection import train_test_split


# %% Read and parse input files
TRAIN_FILE = "./samples/bank.txt"
TEST_FILE = "./samples/bank_test.txt"
INIT_FILE = "./samples/bank_init.txt"
READ_FILE = "./samples/data_banknote_authentication.txt"
df = pd.read_csv(READ_FILE, header=None)
df = df.sample(frac=1)


# %% Add initial line with number of examples, number if inputs, and number of outputs
num_inputs = df.columns.values.shape[0] - 1
num_outputs = 1
num_hidden = 5

fmt = ['%0.3f' for x in range(num_inputs)]
fmt.append('%d')
fmt = ' '.join(fmt)
# %%
train, test = train_test_split(df, test_size=0.2, random_state=31415)
np.savetxt(TRAIN_FILE, train.values, fmt=fmt)
np.savetxt(TEST_FILE, test.values, fmt=fmt)



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
    # weight_vals = rng.uniform(size=(input_dim, output_dim), low=-xavier_lim, high=xavier_lim)
    weight_vals = rng.uniform(size=(input_dim, output_dim), low=-0, high=1)
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

