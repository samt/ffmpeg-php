<?
/*
 * This test script is not part of the automatic regression tests. It serves
 * as a simple manual test script and an example of the syntax for calling
 * the ffmpeg-php functions
 * 
 * To run it from the command line type 'php -q ffmpeg_test.php'or from a 
 * browser * copy this file into your web root and point your browser at it.
 */

$extension = 'ffmpeg';
$extension_soname = $extension . '.' . PHP_SHLIB_SUFFIX;
$extension_fullname = PHP_EXTENSION_DIR . '.' . $extension_soname;


function getDirFiles($dirPath)
{
    if ($handle = opendir($dirPath))
    {
        while (false !== ($file = readdir($handle)))
            if ($file != "CVS" && $file != "." && $file != "..")
                $filesArr[] = trim($dirPath . '/' . $file);

        closedir($handle);
    } 

    return $filesArr;   
}


if (php_sapi_name() != 'cgi') {
    echo '<pre>';
}

// load extension
if(!extension_loaded($extension)) {
    dl($extension_soname) or die("Can't load extension $extension_fullname\n");
}

// print available functions and aliases
echo "\nFunctions available in $extension_fullname extension:\n";
foreach(get_extension_funcs($extension) as $func) {
    echo $func."\n";
}

$class = "ffmpeg_movie";
echo "\nMethods available in class $class:\n";
foreach(get_class_methods($class) as $method) {
    echo $method."\n";
}

// put some movie files into this array to test the ffmpeg functions
$movies = getDirFiles('test_media');

echo "--------------------\n\n";
foreach($movies as $movie) {        
    $mov = new ffmpeg_movie($movie);
    printf("file name = %s\n", $mov->getFileName());
    printf("duration = %s seconds\n", $mov->getDuration());
    printf("frame count = %s\n", $mov->getFrameCount());
    printf("frame rate = %0.3f fps\n", $mov->getFrameRate());
    printf("comment = %s\n", $mov->getComment());
    printf("title = %s\n", $mov->getTitle());
    printf("author = %s\n", $mov->getAuthor());
    printf("copyright = %s\n", $mov->getCopyright());
    printf("frame height = %d pixels\n", $mov->getFrameWidth());
    printf("frame width = %d pixels\n", $mov->getFrameHeight());
    printf("has audio = %s\n", $mov->hasAudio() == 0 ? 'No' : 'Yes');
    printf("get pixel format = %s\n", $mov->getPixelFormat());
    printf("get video bit rate = %d kb/s\n", $mov->getVideoBitRate());
/*    
    while (1) {
        $frame = $mov->getFrame();
        if (!is_resource($frame)) {
            break;
        }
        echo "get frame() $frame" . "\n";
    }
 */    
    printf("get frame = %s\n", $mov->getFrame(10));
    printf("get frame number = %d\n", $mov->getFrameNumber());
    echo "\n--------------------\n\n";
}

if (php_sapi_name() != 'cgi') {
    echo '</pre>';
}

?>
