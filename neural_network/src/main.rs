pub mod training;
pub mod testing;
pub mod data;
use std::io::{self, stdin, Write}; 
 
fn main() {
    let mut user_input = String::new();
    print!("Would you like to train or test a neural network? (1 for training / 2 for testing): ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut user_input).expect("Error reading input");
    let inp = user_input.trim().parse::<i32>().unwrap();
    
    
    if inp == 1{
        train_params();
    }
    
    if inp == 2{
        test_params();
    }
    
}

// Read training parameters
pub fn train_params(){
    let mut train_file = String::new();
    let mut weight_file = String::new();
    let mut out_file = String::new();
    let mut epochs = String::new();
    let mut learning_rate = String::new();
    
    // Read in parameters from user
    print!("Please enter a training file: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut train_file).expect("Error reading input");
    
    print!("Please enter a weight file for the network: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut weight_file).expect("Error reading input");
    
    print!("Please enter an output file: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut out_file).expect("Error reading input");
    
    print!("Epochs: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut epochs).expect("Error reading input");
    let num_epochs = epochs.trim().parse::<i32>().unwrap();
    
    print!("Learning Rate: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut learning_rate).expect("Error reading input");
    let l_rate = learning_rate.trim().parse::<f64>().unwrap();
    
    train(&train_file.trim(), &weight_file.trim(),
                      num_epochs, &out_file.trim(), l_rate);
}

// Function for testing parameters
pub fn test_params(){
    let mut test_file = String::new();
    let mut weight_file = String::new();
    let mut out_file = String::new();
    
    // Read in parameters from user
    print!("Please enter a test file: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut test_file).expect("Error reading input");
    
    print!("Please enter a weight file for the network: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut weight_file).expect("Error reading input");
    
    print!("Please enter an output file: ");
    io::stdout().flush().unwrap();
    stdin().read_line(&mut out_file).expect("Error reading input");
    
    test(&test_file.trim(), &weight_file.trim(), &out_file.trim());

}

// Function to call train program (read in file contents)
pub fn train(train_file: &str, weight_file: &str, epochs: i32,  out_file: &str, learning_rate: f64){
    let x = data::read_data(train_file);
    let w = data::read_data(weight_file);
    let out = training::back_propagation(x, w, &epochs, &learning_rate);
    out.print_data(out_file);
}

// Function to call test program (read in file contents)
pub fn test(test_file: &str, weight_file: &str, out_file: &str){
    let x = data::read_data(test_file);
    let w = data::read_data(weight_file);
    let results = testing::test(x, w);
    testing::print_results(out_file, results);
}