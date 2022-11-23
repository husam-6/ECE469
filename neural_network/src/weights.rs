use std::fs::File;
use std::io::{self, BufRead, Read};
use std::path::Path;

// Helper function to read file line by line
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

pub struct Data{
    pub num_input: f64,
    pub num_hidden: f64,
    pub num_output: f64,
    pub data: Vec<Vec<f64>>

}

// Function to read data in a given file path
pub fn read_data(file_name: &str) -> Data{
    let mut file = File::open(file_name).unwrap();
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    

    let mut data = Vec::new();

    let mut num_input: f64 = 0.0;
    let mut num_hidden: f64 = 0.0;
    let mut num_output: f64 = 0.0;

    // Read file line by line and store data
    if let Ok(lines) = read_lines(file_name) {
        for (i, line) in lines.enumerate(){
            if let Ok(nums) = line {
                // Convert line to vector of doubles
                let nums = nums.trim()
                                     .split(" ").map(|x| x.parse::<f64>()
                                     .unwrap()).collect::<Vec<f64>>();
                if i == 0{
                    num_input = nums[0];
                    num_hidden = nums[1];
                    num_output = nums[2];
                    continue;
                }
                data.push(nums);
            }
        }
    }
    
    // Return data in a struct
    Data {
        num_input,
        num_hidden,
        num_output,
        data,
    }
}