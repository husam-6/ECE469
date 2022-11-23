pub mod weights;
 
fn main() {
    let x = weights::read_data("./samples/wdbc.txt");

    println!("Data size: ({}, {})", x.data.len(), x.data[0].len());
    println!("inputs - {} \noutputs - {} \nhidden - {}", x.num_input, x.num_output, x.num_hidden);
}
