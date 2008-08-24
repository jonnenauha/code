import Data.Char (toUpper, isAlpha, ord, chr)

clean_text :: [Char] -> [Char]
clean_text text = [ toUpper c | c <- text, isAlpha (c)]

section_plaintext :: Int -> [Char] -> [Char]
section_plaintext n text 
    | length text > n = (take n text) ++ [' '] ++ section_plaintext n (drop n text)
    | length text == n = text
    | length text < n = text ++ replicate (n - length text) 'X'

format_plaintext :: [Char] -> [Char]
format_plaintext text = section_plaintext 4 (clean_text text)

decimal_encode_text :: [Char] -> [Int]
decimal_encode_text text = [ ord t - ord 'A' | t <- text ]

decimal_decode_text :: [Int] -> [Char]
decimal_decode_text ns = [ chr (n + ord 'A') | n <- ns ]

rotating_key_cipher :: (Int -> Int -> Int) -> [Int] -> [Int] -> [Int]
rotating_key_cipher op (k:key) (t:text) = (mod (op k t) 26) : (rotating_key_cipher op (key++[k]) text)
rotating_key_cipher _ _ [] = []

cipher :: (Int -> Int -> Int) -> [Char] -> [Char] -> [Char]
cipher op key text = decimal_decode_text 
    (rotating_key_cipher op 
     (decimal_encode_text (clean_text key)) 
     (decimal_encode_text (clean_text text)))

encipher :: [Char] -> [Char] -> [Char]
encipher key text = cipher (+) key text

decipher :: [Char] -> [Char] -> [Char]
decipher key text = cipher (-) key text
