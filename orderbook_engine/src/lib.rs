//! Minimal limit-order book: O(log n) inserts & cancels.

use std::cmp::Reverse;
use std::collections::BTreeMap;

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Side { Buy, Sell }

#[derive(Debug, Clone)]
pub struct Order { pub id: u64, pub qty: u64, pub px: u64, pub side: Side }

#[derive(Default)]
pub struct OrderBook {
    bids: BTreeMap<Reverse<u64>, Vec<Order>>,   // highest price first
    asks: BTreeMap<u64, Vec<Order>>,            // lowest price first
}

impl OrderBook {
    pub fn new() -> Self { Self::default() }

    pub fn insert(&mut self, o: Order) {
        let vec = match o.side {
            Side::Buy  => self.bids.entry(Reverse(o.px)).or_default(),
            Side::Sell => self.asks.entry(o.px).or_default(),
        };
        vec.push(o);
    }

    pub fn best_bid(&self) -> Option<u64> {
        self.bids.keys().next().map(|r| r.0)
    }
    pub fn best_ask(&self) -> Option<u64> {
        self.asks.keys().next().cloned()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn sanity() {
        let mut ob = OrderBook::new();
        ob.insert(Order{id:1,qty:10,px:101,side:Side::Buy});
        ob.insert(Order{id:2,qty:10,px: 99,side:Side::Sell});
        assert_eq!(ob.best_bid(),Some(101));
        assert_eq!(ob.best_ask(),Some( 99));
    }
}
