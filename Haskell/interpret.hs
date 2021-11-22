module Main where
import System.Environment
import Data.List
import Data.Char (chr, ord)
import qualified Data.Map as Map

findBracketIndex :: String -> Int -> Int
findBracketIndex []      _ = error "Brackets do not match"
findBracketIndex ('[':s) n = 1 + findBracketIndex s (n+1)
findBracketIndex (']':_) 0 = 0
findBracketIndex (']':s) n = 1 + findBracketIndex s (n - 1)
findBracketIndex (c:s)   n = 1 + findBracketIndex s n

fillBrackets :: String -> Int -> [(Int, Int)]
fillBrackets [] _ = []
fillBrackets ('[':s) i = let j = 1 + i + findBracketIndex s 0 in (i, j) : (j, i) : fillBrackets s (i+1)
fillBrackets (_:s) i = fillBrackets s (i + 1)

updateAt :: [a] -> Int -> a -> [a]
updateAt l i e = take i l ++ [e] ++ drop (i + 1) l

getOtherBracketPosition :: Maybe Int -> Int
getOtherBracketPosition (Just x) = x
getOtherBracketPosition Nothing  = error "Unmatched bracket"

evaluateBrainfuck :: String -> Int -> [Int] -> Int -> Map.Map Int Int -> IO ()
evaluateBrainfuck src_code code_ptr data_array data_ptr bracket_map
    | code_ptr == length src_code = return ()
    | otherwise =
        let idx = genericIndex in case src_code `idx` code_ptr of
            '>' -> evaluateBrainfuck src_code (succ code_ptr) data_array (succ data_ptr) bracket_map
            '<' -> evaluateBrainfuck src_code (succ code_ptr) data_array (pred data_ptr) bracket_map
            '+' -> let new_data_array = updateAt data_array data_ptr (succ $ data_array `idx` data_ptr) in evaluateBrainfuck src_code (succ code_ptr) new_data_array data_ptr bracket_map
            '-' -> let new_data_array = updateAt data_array data_ptr (pred $ data_array `idx` data_ptr) in evaluateBrainfuck src_code (succ code_ptr) new_data_array data_ptr bracket_map
            '.' -> do
                putChar $ chr $ data_array `idx` data_ptr
                evaluateBrainfuck src_code (succ code_ptr) data_array data_ptr bracket_map
            ',' -> do
                c <- getChar
                let new_data_array = updateAt data_array data_ptr $ ord c
                evaluateBrainfuck src_code (succ code_ptr) new_data_array data_ptr bracket_map
            '[' -> evaluateBrainfuck src_code (if (data_array `idx` data_ptr) == 0 then
                succ $ getOtherBracketPosition $ Map.lookup code_ptr bracket_map
                else succ code_ptr) data_array data_ptr bracket_map
            ']' -> evaluateBrainfuck src_code (if (data_array `idx` data_ptr) /= 0 then
                succ $ getOtherBracketPosition $ Map.lookup code_ptr bracket_map
                else succ code_ptr) data_array data_ptr bracket_map
            c -> error $ "Unkown char: " ++ [c]


main :: IO ()
main = do
    args <- getArgs
    let file_path = if length args /= 1 then
            error "Usage: interpret input-file"
        else args !! 0
    file_content <- readFile file_path
    let src_code = filter (/=' ') $ filter (/='\n') file_content
        bracket_map = Map.fromList $ fillBrackets src_code 0
    evaluateBrainfuck src_code 0 (repeat 0) 0 bracket_map
