pub mod training;
pub mod data;
 
fn main() {
    let x = data::read_data("./samples/wdbc_init.txt");

    // println!("Data size: ({}, {})", x.data.len(), x.data[0].len());
    println!("n0 - {} \nn1 - {} \nn2 - {} ", x.first[0], x.first[1], x.first[2]);
    println!("Data: {:?}", x.data);
}
