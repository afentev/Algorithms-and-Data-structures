extern crate rand;

use libc::INT_MAX;
use rand::distributions::{Distribution, Uniform};

fn median_of5(a: i32, b: i32, c: i32, d: i32, e: i32) -> usize {
  return if b < a {
    if d < c {
      if b < d {
        if a < e {
          if a < d {
            if e < d { 4 } else { 3 }
          } else if c < a { 2 } else { 0 }
        } else if e < d {
          if a < d { 0 } else { 3 }
        } else if c < e { 2 } else { 4 }
      } else if c < e {
        if b < c {
          if a < c { 0 } else { 2 }
        } else if e < b { 4 } else { 1 }
      } else if b < e {
        if a < e { 0 } else { 4 }
      } else if c < b { 2 } else { 1 }
    } else if b < c {
      if a < e {
        if a < c {
          if e < c { 4 } else { 2 }
        } else if d < a { 3 } else { 0 }
      } else if e < c {
        if a < c { 0 } else { 2 }
      } else if d < e { 3 } else { 4 }
    } else if d < e {
      if b < d {
        if a < d { 0 } else { 3 }
      } else if e < b { 4 } else { 1 }
    } else if b < e {
      if a < e { 0 } else { 4 }
    } else if d < b { 3 } else { 1 }
  } else if d < c {
    if a < d {
      if b < e {
        if b < d {
          if e < d { 4 } else { 3 }
        } else if c < b { 2 } else { 1 }
      } else if e < d {
        if b < d { 1 } else { 3 }
      } else if c < e { 2 } else { 4 }
    } else if c < e {
      if a < c {
        if b < c { 1 } else { 2 }
      } else if e < a { 4 } else { 0 }
    } else if a < e {
      if b < e { 1 } else { 4 }
    } else if c < a { 2 } else { 0 }
  } else if a < c {
    if b < e {
      if b < c {
        if e < c { 4 } else { 2 }
      } else if d < b { 3 } else { 1 }
    } else if e < c {
      if b < c { 1 } else { 2 }
    } else if d < e { 3 } else { 4 }
  } else if d < e {
    if a < d {
      if b < d { 1 } else { 3 }
    } else if e < a { 4 } else { 0 }
  } else if a < e {
    if b < e { 1 } else { 4 }
  } else if d < a { 3 } else { 0 };
}

fn median_of3(a: i32, b: i32, c: i32) -> usize {
  if (a > b) ^ (a > c) {
    return 0;
  } else if (b < a) ^ (b < c) {
    return 1;
  }
  2
}

pub fn get_median(arr: &[i32]) -> usize {
  let len = arr.len();
  if len == 1 || len == 2 {
    return 0;
  } else if len == 3 {
    return median_of3(arr[0], arr[1], arr[2]);
  } else if len == 4 {
    let t = median_of5(arr[0], arr[1], arr[2], arr[3], arr[3]);
    if t == 4 {
      return 3;
    }
    return t;
  } else if len == 5 {
    return median_of5(arr[0], arr[1], arr[2], arr[3], arr[4]);
  }
  
  let mut medians: Vec<i32> = vec![0; len / 5 + (len < 5) as usize];
  let sz = medians.len();
  let mut index: usize = 0;
  while index + 5 < len {
    medians[index / 5] = arr[index + median_of5(arr[index], arr[index + 1], arr[index + 2],
                                                arr[index + 3], arr[index + 4])];
    index += 5;
  }
  
  let val = select(&mut medians, sz / 2);
  arr.iter().position(|&s| s == val).unwrap()
}

// this function changes the input vector
pub fn select (v: &mut Vec<i32>, k: usize) -> i32 {
  let mut l = 0;
  let mut r = v.len() - 1;
  
  loop {
    let pivot_ind = l + get_median(&v[l..=r]);
    let pivot = v[pivot_ind];
    v.swap(pivot_ind, r);
    let mut i = l;
    for j in l..r {
      if v[j] < pivot {
        v.swap(i, j);
        i += 1;
      }
    }
    v.swap(r, i);
    
    if i < k {
      l = i + 1;
    } else if i > k {
      r = i - 1;
    } else {
      return v[i];
    }
  }
}


fn main() {
  let mut rng = rand::thread_rng();
  
  const N: usize = 1e6 as usize;
  let value_rnd = Uniform::from(1..INT_MAX);
  let index_rnd = Uniform::from(0..N);
  
  let mut array: [i32; N] = [0; N];
  for i in 0..N {
    let throw = value_rnd.sample(&mut rng);
    array[i] = throw;
  }
  
  // println!("{:?}", array);
  
  let mut x: Vec<i32> = Vec::from(array);
  
  let k = index_rnd.sample(&mut rng);
  
  use std::time::Instant;
  let now = Instant::now();
  let ans = select(&mut x.clone(), k);
  let elapsed = now.elapsed();
  
  x.sort();
  let correct = x[k];
  println!("{}", ans == correct);
  
  // println!("{:?}", x);
  println!("K = {}", k);
  println!("Ans: {}", ans);
  println!("Correct: {}", correct);
  
  println!("Elapsed: {:.2?}", elapsed);
}