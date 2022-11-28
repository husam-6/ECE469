pub mod training;
pub mod testing;
pub mod data;
 
fn main() {
    // let x = data::read_data("./samples/wdbc.txt");
    // let w = data::read_data("./samples/wdbc_init.txt");
    let x = data::read_data("./samples/wdbc_test.txt");
    let w = data::read_data("./samples/wdbc_final.txt");
    
    
    // let out = training::back_propagation(x, w, &100, &0.1);
    // out.print_data("out.txt");
    let results = testing::test(x, w);
    testing::print_results("res.txt", results);
}
