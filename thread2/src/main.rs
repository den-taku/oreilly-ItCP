use std::thread::spawn;

fn hello() {
    println!("Hello World!");
}

fn main() {
    let _ = spawn(hello).join();

    let h = || println!("Hello World");
    let _ = spawn(h).join();

    my_func12();
}

fn my_func12() {
    let v = 10;
    let f = move || v * 2;

    let result = spawn(f).join();
    println!("result = {:?}", result);

    match spawn(|| panic!("I'm panicked!")).join() {
        Ok(_) => unreachable!(),
        Err(a) => println!("failed: {:?}", a.downcast_ref::<&str>())
    }
}