import Data.Array
import Data.Char
import Control.Monad.State

type Hash = Array Int (Maybe String)
type HashT = StateT Hash IO ()

setup :: Hash
setup = array (0,100) [(i, Nothing) | i <- [0..100]]

runHash :: Int -> HashT
runHash 0 = return ()
runHash n = do
    h <- get
    (com, _:s) <- liftIO $ getLine >>= return . break (==':')
    let h' = case com of
                "ADD" -> insert s h
                "DEL" -> remove s h
                _     -> error "unknown command"
    put h'
    runHash (n - 1)

find :: String -> Hash -> Int
find s h = find' s h j 0 (-1)
           where j = hash s
                 find' s h j 20 x = x
                 find' s h j n x = 
                    case h!j' of
                        Nothing -> find' s h j (n+1) (if x < 0 then j' else x)
                        Just s' -> if s == s' 
                                    then find' s h j (n+1) j' --(if x < 0 then j' else x)
                                    else find' s h j (n+1) x
                    where j' = hash' j n

apply :: (Int -> String -> Hash -> Hash) -> String -> Hash -> Hash
apply f s h = let ix = find s h in
         if ix >= 0 then
            f ix s h 
         else h


insert :: String -> Hash -> Hash
insert = apply (\ix s h -> case h!ix of Nothing -> (//) h [(ix, Just s)] ; Just s  -> h)

remove :: String -> Hash -> Hash
remove = apply (\ix s h -> case h!ix of 
                    Nothing -> h 
                    Just s' -> if s == s' 
                               then (//) h [(ix, Nothing)]
                               else h)

hash :: String -> Int
hash s = (19 *  (foldl (\a (c, i) -> (+) a $ i * ( ord c)) 0 $ zip s [1..] )) `mod` 101

hash' :: Int -> Int -> Int
hash' n j = (n + j*j + 23*j) `mod` 101

printH :: Hash -> IO ()
printH = mapM_ putStrLn . map f . filter g . assocs
          where f (i, Just s) =  show i ++ ":" ++ s
                g (i, Nothing) = False
                g (i, Just s)  = True

sizeH :: Hash -> Int
sizeH = length . filter g . elems
          where g Nothing = False
                g (Just s)   = True

main' :: Int -> IO ()
main' 0 = return ()
main' n = do
    t <- getLine >>= return . read
    r <- execStateT (runHash t) setup
    putStrLn $ show $ sizeH r
    printH r
    main' (n-1)

main = do 
    n <- getLine
    main' $ read n
