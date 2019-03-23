--------------------------------------------------------------------------------------------
--- Authoren :: Amro Hendawi -> Leon Marius Moll -> Saarujan Sritharan -> DeathStacksBot ---
--------------------------------------------------------------------------------------------
-----v 1.2-----

--- module (NICHT AENDERN!)
module DeathStacksBot where
--- imports (NICHT AENDERN!)
import Data.Char
import Util

--- external signatures (NICHT AENDERN!)
getMove :: String -> String
listMoves :: String -> String
getMove state = (movesList state) !! (((length(movesList state))*7+13) `mod` (length(movesList state)))
listMoves state = "[" ++ (init(concat(map (++ ",") (rmDplct (Util.splitOn "," (init((allVHMoves state) ++ (allDiagonalMoves state)))))))) ++ "]"
movesList state = (rmDplct (Util.splitOn "," (init((allVHMoves state) ++ (allDiagonalMoves state))))) 
--- YOUR IMPLEMENTATION STARTS HERE ---
splitRowEnd :: [Char] -> [[Char]]
splitRowEnd state = Util.splitOn "/" state

splitColEnd :: [Char] -> [[Char]]
splitColEnd state = Util.splitOn "," state

replaceO [] = []
replaceO (x:xs) = 
     if x == '/' 
     then ',' : replaceO xs 
     else x : replaceO xs

-- extract player stacks locations and player color
stacksCoords state = zip ["a6","b6","c6","d6","e6","f6","a5","b5","c5","d5","e5","f5","a4","b4","c4","d4","e4","f4","a3","b3","c3","d3","e3","f3","a2","b2","c2","d2","e2","f1","a1","b1","c1","d1","e1","f1"] (splitColEnd (concat (splitRowEnd (init(init(replaceO state))))))
stacksFields state = let y = stacksCoords state in filter (((/="")).snd) y
playerFields state = let x = stacksFields state in filter (((==(last(state))).head).snd) x
playerFieldsTooTall state = let x = playerFields state in filter (((>4).length).snd) x
-------------- return only stack higher than 4 if exists, else return all player stacks
playerFieldsTooTalll state | ((playerFieldsTooTall state) /= []) = playerFieldsTooTall state
                            | otherwise = playerFields state
getPlayer string = last (string)

stringToList state = splitColEnd (concat (splitRowEnd (init(init(replaceO state)))))
        
charToInt :: Char -> Int
charToInt c  | c =='a' = 1 | c =='b' = 2 | c =='c' = 3 | c =='d' = 4 | c =='e' = 5 | c =='f' = 6
                
zahlToInt :: Char -> Int
zahlToInt c  | c == '1' = 1 | c == '2' = 2 | c == '3' = 3 | c == '4' = 4 | c == '5' = 5 | c == '6' = 6

intToZahl :: Int -> Char
intToZahl i  | i == 1 = '1' | i == 2 = '2' | i == 3 = '3' | i == 4 = '4' | i == 5 = '5' | i == 6 = '6'

intToChar :: Int -> Char
intToChar i  | i == 1 = 'a' | i == 2 = 'b' | i == 3 = 'c' | i == 4 = 'd' | i == 5 = 'e' | i == 6 = 'f'

intToString :: Int -> [Char]
intToString i  | i == 0 = "0" | i == 1 = "1" | i == 2 = "2" | i == 3 = "3" | i == 4 = "4" | i == 5 = "5" | i == 6 = "6"
               | i == 7 = "7" | i == 8 = "8" | i == 9 = "9" | i == 10 = "10" | i == 11 = "11" | i == 12 = "12"
               | i == 13 = "13" | i == 14 = "14" | i == 15 = "15" | i == 16 = "16" | i == 17 = "17" | i == 18 = "18"
               | i == 19 = "19" | i == 20 = "20" | i == 21 = "21" | i == 22 = "22" | i == 23 = "23" | i == 24 = "24"

intToStrChar :: Int -> [Char]
intToStrChar i  | i == 1 = "a" | i == 2 = "b" | i == 3 = "c" | i == 4 = "d" | i == 5 = "e" | i == 6 = "f"
                | i == 7 = "a"  | i == 8 = "b" | i == 9 = "c" | i == 10 = "d" | i == 11 = "e" | i == 12 = "f"

stringToInt :: [Char] -> Int
stringToInt i  | i == "1" = 1 | i == "2" = 2 | i == "3" = 3 | i == "4" = 4 | i == "5" = 5 | i == "6" = 6

charToString :: Char -> [Char]
charToString i  | i == '1' = "a" | i == '2' = "b" | i == '3' = "c" | i == '4' = "d" | i == '5' = "e" | i == '6' = "f"

charLetterToString :: Char -> [Char]
charLetterToString i  | i == 'a' = "a" | i == 'b' = "b" | i == 'c' = "c" | i == 'd' = "d" | i == 'e' = "e" | i == 'f' = "f"

interval x lengthX = [x..lengthX]
mapPairs xs ys =
    do
        x <- xs
        y <- ys
        return (x,y)
-----------------------vertical horizontal normal & mirroring section -----------
case_X a l  | (head(fst(a)))/=(intToChar(move_X a l))  = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(move_X a l)) ++ (tail(fst(a)))
            | otherwise             = ""
caseX a l   | (head(fst(a)))/=(intToChar(moveX a l))   = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(moveX a l)) ++ (tail(fst(a)))
            | otherwise             = ""
caseY a l   | (tail(fst(a))) /= (intToString(moveY a l)) = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (charLetterToString(head(fst(a)))) ++ (intToString(moveY a l))
            | otherwise             = ""
case_Y a l  | (tail(fst(a))) /= (intToString(move_Y a l)) = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (charLetterToString(head(fst(a)))) ++ (intToString(move_Y a l))
            | otherwise             = ""

fourCasesVH a l = (caseX a l) ++ "," ++ (case_X a l) ++ "," ++ (caseY a l) ++ "," ++ (case_Y a l)
--- VH moves with interval filtered to match TooTall rule
fourCasesVHGlobal a = let x = (filter(>=(length(snd(a)))-4)(interval 1 (length(snd(a))))) in map (fourCasesVH a ) x
vhToList a = concat (map (Util.splitOn ",") (fourCasesVHGlobal a))
vhToListF a = let y = vhToList a in filter (/="") y

allVHMoves state = concat (map (++ ",") (concat(map (vhToListF) (playerFieldsTooTalll state))))
--------------------- diagonal normal & mirroring section -------------------------------------------
case_XY a l  | ((head(fst(a)))/=(intToChar(move_X a l)) && (tail(fst(a)))/=(intToString(moveY a l))) = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(move_X a l)) ++ (intToString(moveY a l))
            | otherwise             = ""
caseXY a l   | ((head(fst(a)))/=(intToChar(moveX a l)) && (tail(fst(a)))/=(intToString(moveY a l)))  = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(moveX a l)) ++ (intToString(moveY a l))
            | otherwise             = ""
caseX_Y a l   | ((head(fst(a)))/=(intToChar(moveX a l)) && (tail(fst(a))) /= (intToString(move_Y a l))) = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(moveX a l)) ++ (intToString(move_Y a l))
            | otherwise             = ""
case_X_Y a l  | ((head(fst(a)))/=(intToChar(move_X a l)) && (tail(fst(a))) /= (intToString(move_Y a l))) = (fst(a)) ++ "-"  ++ (intToString(l)) ++ "-" ++ (intToStrChar(move_X a l)) ++ (intToString(move_Y a l))
            | otherwise             = ""

fourCases a l = (caseXY a l) ++ "," ++ (case_X_Y a l) ++ "," ++ (case_XY a l) ++ "," ++ (caseX_Y a l)
--- diagonal moves with interval filtered to match TooTall rule
fourCasesGlobal a = let x = (filter(>=(length(snd(a)))-4)(interval 1 (length(snd(a))))) in map (fourCases a ) x
allDiagonalMovess state = concat (map (++ ",") (concat(map (fourCasesGlobal) (playerFieldsTooTalll state))))
allDiagonalMoves state = concat(map(++",")(filter(/="")(splitOn(",")(allDiagonalMovess state))))
------------------------------------------------------------------------------------
------ the four lines below calculate target in all directions and dimensions-------
moveX a l = calcPlus (charToInt(head(fst(a)))) (l`mod`10)
moveY a l = calcPlus (stringToInt(tail(fst(a)))) (l`mod`10)
move_X a l = calcMinus (charToInt(head(fst(a)))) (l`mod`10)
move_Y a l = calcMinus (stringToInt(tail(fst(a)))) (l`mod`10)
------------------------------------------------------------------------------------
------------------------------distance factory--------------------------------------
-- move calculator (x/y++) or (x/y--) -> steps
calcPluss :: Int -> Int ->Int
calcPluss x l = if (x + l) > 6
                    then 6 - (x + l - 6)
                    else (x + l)

calcMinuss :: Int -> Int ->Int
calcMinuss x l = if (x - l) < 1
                    then 1 - (x - l - 1)
                    else (x - l)
-- move loop works for infinit positiv numbers
calcPlus x l = if (calcPluss x l) < 1
                then (calcMinus 0 (abs(calcPluss x l)))
                else (calcPluss x l)

calcMinus x l = if (calcMinuss x l) > 6
                then (calcPlus 0 (abs(calcMinuss x l)))
                else (calcMinuss x l)
----------------------------------------------------------------------------------------
filterZero [] = []
filterZero (x:xs)   | any (=='0') x   = filterZero xs
                    | otherwise     = x : filterZero xs

rmDplct :: Eq a => [a] -> [a]
rmDplct [] = []
rmDplct (x:xs)   | x `elem` xs   = rmDplct xs
                | otherwise     = x : rmDplct xs