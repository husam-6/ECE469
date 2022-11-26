use crate::data::Data;

// Activtion functions - using sigmoid now for simple derivative
pub fn sigmoid(x: &f64) -> f64{
    1.0 / (1.0 + (-x).exp())
}

pub fn d_sigmoid(x: &f64) -> f64{
    sigmoid(x) * (1.0 - sigmoid(x))
}

pub fn back_propagation(training_set: Data, weights: Data, epochs: &i32) -> Vec<Vec<f64>>{
    // Separate weights for easier use
    let w = weights.data;

    // Save the dimensions of the training data (stored in a Data struct)
    let rows = training_set.first[0] as usize;
    // let cols = training_set.first[1] as i32;

    // Save constants in given data
    let num_outputs = training_set.first[2] as usize;
    let num_inputs = weights.first[0] as usize;
    let num_hidden = weights.first[1] as usize;
    
    // To initalize the neural network
    let input = vec![0.0; num_inputs];
    let hidden = vec![0.0; num_hidden];
    let output = vec![0.0; num_outputs];

    // The actual neural network nodes
    let mut network = vec![input, hidden, output];

    // Repeat training for number of epochs specified
    for _ in 0..(*epochs){
        for i in 0..rows{
            let mut in_ = vec![vec![0.0; num_hidden], vec![0.0; num_outputs]];

            // Copy training example into input nodes in the network
            for k in 0..num_inputs{
                network[0][k] = training_set.data[i][k];
            } 

            // Forward prop
            for l in 1..3{
                for j in 0..network[l].len(){
                    let mut sum = 0.0; 
                    // Multiply weight of link bw previous layer and current node * node value
                    for k in 0..w[l-1].len(){
                        sum += w[l-1][k] * network[l][k];
                    }
                    in_[l - 1][j] = sum;    
                    network[l][j] = sigmoid(&sum);
                }
            }

            // // Propagate deltas backwards
            // for j in 0..network[2].len(){
            //     // delta[j] = 
            // }


        }
    }
    network
}