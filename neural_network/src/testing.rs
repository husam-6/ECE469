use crate::data::Data;
use crate::training::sigmoid;
use std::io::Write;

#[derive(Clone)]
#[derive(Debug)]
#[derive(Copy)]
pub struct Confusion{
    pub a: f64,
    pub b: f64,
    pub c: f64,
    pub d: f64
}

pub struct Metrics{
    pub acc: f64,
    pub precision: f64,
    pub recall: f64,
    pub f1: f64
}

impl Confusion{
    pub fn calc_metrics(&self) -> Metrics{
        let acc = (self.a + self.d) / (self.a + self.b + self.c + self.d);
        let precision = self.a / (self.a + self.b);
        let recall = self.a / (self.a + self.c);
        let f1 = (2.0 * precision * recall) / (precision + recall);
        Metrics { acc, precision, recall, f1 }
    }
}

impl Default for Confusion{
    fn default() -> Confusion{
        Confusion {
            a: 0.0,
            b: 0.0,
            c: 0.0,
            d: 0.0
        }
    }
}

impl Default for Metrics{
    fn default() -> Metrics{
        Metrics {
            acc: 0.0,
            precision: 0.0,
            recall: 0.0,
            f1: 0.0
        }
    }
}

impl Confusion{
    pub fn add(&mut self, b: Confusion){
        self.a += b.a;
        self.b += b.b;
        self.c += b.c;
        self.d += b.d;
    }
    pub fn as_array(&self) -> [f64; 4] {
        [self.a, self.b, self.c, self.d]
    }
    pub fn join(&self) -> String {
        format!("{} {} {} {}", self.a, self.b, self.c, self.d)
    }
}

impl Metrics{
    pub fn add(&mut self, b: Metrics){
        self.acc += b.acc;
        self.precision += b.precision;
        self.recall += b.recall;
        self.f1 += b.f1;
    }
    pub fn as_array(&self) -> [f64; 4] {
        [self.acc, self.precision, self.recall, self.f1]
    }
    pub fn join(&self) -> String {
        format!("{:.3} {:.3} {:.3} {:.3}", self.acc, self.precision, self.recall, self.f1)
    }
}



pub fn test(test_set: Data, weights: Data) -> Vec<Confusion>{
    // Separate weights for easier use
    let w = weights.data;

    // Save the dimensions of the training data (stored in a Data struct)
    let rows = test_set.first[0] as usize;

    // Save constants in given data
    let num_outputs = test_set.first[2] as usize;
    let num_inputs = weights.first[0] as usize;
    let num_hidden = weights.first[1] as usize;
    
    // To initalize the neural network
    let mut network = vec![vec![0.0; num_inputs], vec![0.0; num_hidden], vec![0.0; num_outputs]];

    // Weight bias
    let w_b = -1.0;

    // To store results 
    let mut res:Vec<Confusion> = vec![Confusion { ..Default::default() }; num_outputs];

    // Loop through test set 
    for example in 0..rows{
        let mut w_idx = 0;
        
        // Copy training example into input nodes in the network
        for k in 0..num_inputs{
            network[0][k] = test_set.data[example][k];
        } 
        
        // Forward prop
        for l in 1..3{
            for j in 0..network[l].len(){
                // Multiply weight of link bw previous layer and current node * node value
                let mut sum = 0.0; 
                sum += w_b * w[w_idx][0];
                for k in 0..network[l - 1].len(){
                    sum += w[w_idx][k + 1] * network[l - 1][k];
                }
                network[l][j] = sigmoid(&sum);
                w_idx+=1;
            }
        }
        
        // Keep track of counts for confusion matrices
        let truth = &test_set.data[example][num_inputs..];
        let result = &network[2];
        for i in 0..truth.len(){
            let est = result[i].round();

            // Expected 1
            if truth[i] == 1.0 {
                if est == 1.0 {         // Predicted 1
                    res[i].a += 1.0; 
                }
                if est == 0.0 {
                    res[i].c += 1.0;    // Predicted 1
                }
            }
            
            // Expected 0
            if truth[i] == 0.0 {
                if est == 1.0 {         // Predicted 1
                    res[i].b += 1.0; 
                }
                if est == 0.0 {
                    res[i].d += 1.0;    // Predicted 1
                }
            }
        }
    }
    res
}

pub fn print_results(output: &str, res: Vec<Confusion>){
    let mut file = std::fs::File::create(output).expect("create failed");
    let mut macro_sum = Metrics { ..Default::default() };
    let mut micro_sum = Confusion { ..Default::default() };

    // Calculate and print metrics for each output node
    for i in 0..res.len(){
        // Calculated metrics
        let c_metrics = res[i].calc_metrics();
        // Formatted strings
        let values = res[i].join();
        let metrics = c_metrics.join();
        file.write_all(format!("{} {}\n", values, metrics).as_bytes()).expect("Write Failed");
        macro_sum.add(c_metrics);
        micro_sum.add(res[i]);
    }
    
    // Calculate averages (scale values)
    let mut micro_arr = micro_sum.as_array();
    let mut macro_arr = macro_sum.as_array();
    for i in 0..micro_arr.len(){
        micro_arr[i] = micro_arr[i] / res.len() as f64;
        macro_arr[i] = micro_arr[i] / res.len() as f64;
    }
    
    let micro = micro_sum.calc_metrics();

    file.write_all(format!("{}\n", micro.join())
                                .as_bytes()).expect("Write Failed");
    
    // Scale for macro averages
    file.write_all(format!("{}\n", macro_sum.join())
                                .as_bytes()).expect("Write Failed");

}