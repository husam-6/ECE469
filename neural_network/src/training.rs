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
    let mut network = vec![vec![0.0; num_inputs], vec![0.0; num_hidden], vec![0.0; num_outputs]];

    // Weight bias
    let w_b = -1.0;

    // Repeat training for number of epochs specified
    for _ in 0..(*epochs){
        for example in 0..rows{
            let mut in_ = vec![vec![0.0; num_hidden], vec![0.0; num_outputs]];

            // Copy training example into input nodes in the network
            for k in 0..num_inputs{
                network[0][k] = training_set.data[example][k];
            } 
            
            // Forward prop
            let mut w_idx = 0;
            for l in 1..3{
                for j in 0..network[l].len(){
                    // Multiply weight of link bw previous layer and current node * node value
                    let mut sum = 0.0; 
                    sum += w_b * w[w_idx][0];
                    for k in 0..network[l - 1].len(){
                        sum += w[w_idx][k + 1] * network[l - 1][k];
                    }
                    in_[l - 1][j] = sum;
                    network[l][j] = sigmoid(&sum);
                    w_idx+=1;
                }
            }
            
            // Initialize errors... Don't need error for input nodes
            let mut delta = vec![vec![0.0; num_hidden], vec![0.0; num_outputs]];
            
            // Propagate deltas backwards - calculate error for output nodes
            for j in 0..network[2].len(){
                delta[1][j] = d_sigmoid(&in_[1][j]) * (training_set.data[example][num_inputs + j] - network[2][j]);
            }

            // Calculate deltas for hidden nodes
            for i in 0..network[1].len(){
                let mut sum = 0.0; 
                for k in 0..network[2].len(){
                    sum += w[num_hidden + k][i + 1] * delta[1][k];
                }
                delta[0][i] = d_sigmoid(&in_[0][i]) * sum;
            }
            
            // Update each weight
            let mut l = 0;
            let mut n = 0;  
            for i in 0..w.len(){
                for j in 0..w[i].len(){
                    if j == 0{
                        w[i][j] = w[i][j] + learning_rate * w_b * delta[l][n];
                        continue;
                    }
                    w[i][j] = w[i][j] + learning_rate * network[l][j - 1] * delta[l][n];
                }
                // Increment layer
                if i == num_hidden - 1{
                    l+=1;
                    n=0;
                    continue; 
                }
                n+=1;
            }
        }
    }
    weights.data = w;
    weights
}