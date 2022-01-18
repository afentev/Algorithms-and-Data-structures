extern crate rand;

use std::collections::HashMap;
use rand::distributions::{Distribution, Uniform};

fn main() {
  let mut rng = rand::thread_rng();
  
  const N: usize = 10;
  let value_rnd = Uniform::from(1..100);
  
  let mut a: [i32; N] = [0; N];
  for i in 0..N {
    let throw = value_rnd.sample(&mut rng);
    a[i] = throw;
  }
  
  let mut dp = a.clone();
  for i in 0..dp.len() {
    dp[i] = i32::max_value();
  }
  dp[0] = a[0];
  
  let mut parents: HashMap<i32, i32> = HashMap::new();
  for i in 0..a.len() {
    let value = a[i];
    let index = dp.binary_search(&value).unwrap_or_else(|i| i);
    if value < dp[index] {
      dp[index] = value;
      if index > 0 {
        parents.insert(value, dp[index - 1]);
      }
    }
  }
  
  let mut ans = 0;
  for i in 0..dp.len() {
    if dp[i] == i32::max_value() {
      break;
    }
    ans = i;
  }
  
  println!("{:?}", a);
  print!("Length: {}\nSeq: ", ans + 1);
  let mut val = dp[ans];
  let mut seq: Vec<i32> = Vec::with_capacity(ans);
  while parents.contains_key(&val) {
    seq.push(val);
    val = *parents.get(&val).unwrap();
  }
  seq.push(val);
  seq.reverse();
  for item in seq {
    print!("{} ", item);
  }
  println!();
}