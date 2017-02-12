sumOfMultThreeFive :: Int->Int
sumOfMultThreeFive max = sumList [x|x < max, x `mod` 3 == 0 || x `mod` 5 == 0]
  where
    sumList :: [Int] -> Int
    sumList [] = 0
    sumList (x:xs) = x + sumList xs