use crate::data::Data;

// Activtion functions - using sigmoid now for simple derivative
pub fn sigmoid(x: &f64) -> f64{
    1.0 / (1.0 + (-x).exp())
}

pub fn d_sigmoid(x: &f64) -> f64{
    sigmoid(x) * (1.0 - sigmoid(x))
}

pub fn back_propagation(training_set: Data, mut weights: Data, epochs: &i32, learning_rate: &f64) -> Data{
    // Separate weights for easier use
    let mut w = weights.data;

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

    // Weight bias
    let w_b = -1.0;

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
                    // println!("weights: {:?}", w);
                    // println!("Size of weights: {}, size of nodes {}", w[l-1].len(), network[l - 1].len());
                    sum += w_b * w[j][0];
                    for k in 0..network[l - 1].len(){
                        sum += w[j][k + 1] * network[l - 1][k];
                    }
                    in_[l - 1][j] = sum;    
                    network[l][j] = sigmoid(&sum);
                }
            }
            
            // Initialize errors... Don't need error for input nodes
            let mut delta = vec![vec![0.0; num_hidden], vec![0.0; num_outputs]];
            
            // Propagate deltas backwards - loop through output nodes and calculate loss
            for j in 0..network[2].len(){
                delta[1][j] = d_sigmoid(&in_[1][j]) * (training_set.data[i][num_inputs + j] - network[2][j]);
            }

            // Calculate deltas for hidden nodes
            for i in 0..network[1].len(){
                let mut sum = 0.0; 
                
                sum += w_b * w[i][0];
                for k in 0..network[2].len(){
                    sum += w[i][k + 1] * delta[1][k];
                }
                delta[0][i] = d_sigmoid(&in_[0][i]) * sum;
            }
            
            // Update each weight
            for l in 0..delta.len(){
                for n in 0..delta[l].len(){
                    for i in 0..w.len(){
                        for j in 0..w[i].len(){
                            if i == 0{
                                w[i][j] = w[i][j] + learning_rate * w_b * delta[l][n];
                                continue;
                            }
                            w[i][j] = w[i][j] + learning_rate * network[l][i - 1] * delta[l][n];
                        }
                    }
                }
            }
        }
    }
    weights.data = w;
    weights
}