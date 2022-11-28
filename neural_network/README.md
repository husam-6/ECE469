## Artificial Intelligence Project 2

### Husam Almanakly

This crate implements a neural network, from scratch, without the use of linear algebra / 
matrix operations using direct forward and back propagation.

## Rust Installation and Execution

If you don't have Rust installed, you can do so following the instructions outlined [here](https://doc.rust-lang.org/book/ch01-01-installation.html)

Once installed, run the script with

<pre>
➜  neural_network git:(main) ✗ cargo run
    Finished dev [unoptimized + debuginfo] target(s) in 0.04s
     Running `target/debug/neural_network`
Would you like to train or test a neural network? (1 for training / 2 for testing):
</pre>

You will be prompted if you want to trian or test a neural network.


## Training
If you choose to train, the program will ask for 5 paramters as shown in the example below:

<pre>
Please enter a training file: ./samples/wdbc.txt
Please enter a weight file for the network: ./samples/wdbc_init.txt
Please enter an output file: out.txt
Epochs: 100
Learning Rate: 0.1
</pre>

The weight files specify the number of inputs, hidden nodes, and number of outputs. The weights for each
edge connecting any given node to another is also given. The expected pre-processed format is the same as
outlined in the assignment:

<pre>
•	The first line will contain three integers, separated by single spaces, representing the number of input nodes (Ni), the number of hidden nodes (Nh), and the number of output nodes (No).
•	The next Nh lines specify the weights of edges pointing from input nodes to hidden nodes. The first of these lines specifies the weights of edges entering the first hidden node; the second line specifies the weights of edges entering the second hidden node; etc. Each of these Nh lines specifies Ni + 1 weights, which will be floating-point numbers separated by single spaces. These weights include the bias weight which is attached to a fixed input that always has its activation set to -1. (Note that the fixed input should be -1, not +1, so we are not following the convention of the current edition of the textbook.) Using a fixed input of -1 makes the bias weight equivalent to a threshold to which the total of the true weighted input can be compared. For each hidden node, the first value represents the bias weight, and the next Ni values represent the weights of edges from the input nodes to the hidden node.
•	The next No lines specify the weights of edges pointing from hidden nodes to output nodes. The first of these lines specifies the weights of edges entering the first output node; the second line specifies the weights of edges entering the second output node; etc. Each of these No lines specifies Nh + 1 weights, which will be floating-point numbers separated by single spaces. These weights include the bias weight which is attached to a fixed input that always has its activation set to -1. For each output node, the first value represents the bias weight, and the next Nh values represent the weights of edges from the hidden nodes to the output node.
</pre>

The training file contains values for each input in the network, along with a labelled output. The format is also very similar; also matching the
convention in the assignment: 

<pre>
The first line of this file contains three integers separated by spaces: the number of training examples, Ni, and No (I guarantee that Ni and No will match the neural network being trained). Every other line of the file specifies a single example and contains Ni floating-point inputs (the values of the example's input attributes) followed by No Boolean outputs (each will be 0 or 1)
</pre>

Output files for training will match this convention

## Testing

If you choose to test, you will be prompted for 3 parameters:

<pre>
➜  neural_network git:(main) ✗ cargo run
    Finished dev [unoptimized + debuginfo] target(s) in 0.20s
     Running `target/debug/neural_network`
Would you like to train or test a neural network? (1 for training / 2 for testing): 2
Please enter a test file: ./samples/wdbc_test.txt
Please enter a weight file for the network: out.txt
Please enter an output file: r.txt
</pre>

The conventions will match for the test set and the weight file (for which you should provide the name
of the output file you used when training the network)

The output file for testing is different however. It specifies 4 metrics for each output level: accuracy,
precision, recall, and f1. The confusion matrix values are first written for each output. The last two lines
in the file represent micro and macro averages of all the output metrics. An example output from the above run
is shown below:

<pre>
167 8 2 92 0.963 0.954 0.988 0.971
0.963 0.954 0.988 0.971
0.963 0.954 0.988 0.971
</pre>

(Note only 1 output variable is needed in this dataset, so there is one output line and a macro 
and micro average which are identical)