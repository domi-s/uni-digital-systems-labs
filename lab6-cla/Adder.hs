-- Digital Hardware practical
-- Mike Spivey
-- Hilary Term, 2004

{-# LANGUAGE NPlusKPatterns #-}

module Adder where

-- Representing integers

infix 1 :@

data Timed a = a :@ Int deriving Show

type Bit = Timed Int

pegged :: a -> Timed a
pegged x = x :@ 0

timed2 :: (a -> b -> w) -> Timed a -> Timed b -> Timed w
timed2 f (x :@ tx) (y :@ ty) = f x y :@ (max tx ty + 1)

timed3 :: (a -> b -> c -> w) -> Timed a -> Timed b -> Timed c -> Timed w
timed3 f (x :@ tx) (y :@ ty) (z :@ tz) = f x y z :@ (max tx (max ty tz) + 1)

{-
rep :: Int -> Integer -> [Bit]
bin :: [Bit] -> Integer
-- TODO: definitions of rep and bin
-}

-- Useful gates

{-
majority :: Bit -> Bit -> Bit -> Bit
parity :: Bit -> Bit -> Bit -> Bit
-- TODO: definitions of majority, parity

full_adder :: Bit -> (Bit, Bit) -> (Bit, Bit)
-- TODO: definition of full_adder
-}

-- Ripple-carry adder

{-
ripple :: (a -> b -> (c, a)) -> a -> [b] -> [c]
-- TODO: definition of ripple

adder :: [Bit] -> [Bit] -> [Bit]
adder as bs = ripple full_adder (0, 0) (zip as bs)
-}

-- Introducing carry status

data KPG = K | P | G
type Flag = (KPG, Int)

{-
kpg :: (Bit, Bit) -> Flag
sumbit :: Flag -> (Bit, Bit) -> Bit
bun :: Flag -> Flag -> Flag
-- TODO: definitions of kpg, sumbit, bun

rip_cl_adder :: [Bit] -> [Bit] -> [Bit]
rip_cl_adder as bs = ripple kpg_adder (K, 0) (zip as bs)

kpg_adder :: Flag -> (Bit, Bit) -> (Bit, Flag)
-- TODO: definition of kpg_adder
-}

-- Parallel prefix.

type Prefix_fun a = (a -> a -> a) -> a -> [a] -> [a]

{-
cl_adder :: Prefix_fun Flag -> [Bit] -> [Bit] -> [Bit]
cl_adder pfx as bs = zipWith sumbit carries inputs
  where 
    carries = pfx bun (K, 0) (map kpg inputs)
    inputs = zip as bs

rip_prefix :: Prefix_fun a
-- TODO: definition of rip_prefix
-}

{-
par_prefix :: Prefix_fun a
par_prefix (%) u [x] = [u]
par_prefix (%) u xs = 
  concat (zipWith g es (par_prefix (%) u (zipWith (%) es os)))
    where 
      es = evens xs; os = odds xs
      g x y = [y, y % x]

-- TODO: definitions of evens, odds
-}
