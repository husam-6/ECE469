pub mod training;
pub mod data;
 
fn main() {
    let x = data::read_data("./samples/wdbc.txt");
    let w = data::read_data("./samples/wdbc_init.txt");
    // println!("n0 - {} \nn1 - {} \nn2 - {} ", x.first[0], x.first[1], x.first[2]);
    // println!("Init: {:?}", w.data);
    let out = training::back_propagation(x, w, &100, &0.1);
    out.print_data("out.txt");
}
