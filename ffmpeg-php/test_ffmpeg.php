<?
/*
 * This test script is not part of the automatic regression tests. It serves
 * as a simple manual test script and an example of the syntax for calling
 * the ffmpeg-php functions.
 * 
 * To run it from the command line type 'php -q ffmpeg_test.php ' or from a 
 * browser copy this file into your web root and point your browser at it.
 */

$extension = "ffmpeg";
$extension_soname = $extension . "." . PHP_SHLIB_SUFFIX;
$extension_fullname = PHP_EXTENSION_DIR . "/" . $extension_soname;

// load extension
if (!extension_loaded($extension)) {
    dl($extension_soname) or die("Can't load extension $extension_fullname\n");
}

if (php_sapi_name() != 'cli') {
    echo '<pre>';
}

printf("ffmpeg-php version string: %s\n", FFMPEG_PHP_VERSION_STRING);
printf("ffmpeg-php build date string: %s\n", FFMPEG_PHP_BUILD_DATE_STRING);
printf("libavcodec build number: %d\n", LIBAVCODEC_BUILD_NUMBER);
printf("libavcodec version number: %d\n", LIBAVCODEC_VERSION_NUMBER);

printf("ffmpeg-php gd enabled: %s\n", FFMPEG_PHP_GD_ENABLED ? 'TRUE' : 'FALSE');

// get movie methods minus the contructor
$movie_methods = array_shift(print_class_methods("ffmpeg_movie"));

$frame_methods = print_class_methods("ffmpeg_frame");

// get an array for movies from the test media directory 
$movies = getDirFiles(dirname(__FILE__) . '/tests/test_media');

echo "--------------------\n\n";
foreach($movies as $movie) {        
    printf("TEST FILE: %s\n", basename($movie));
    $mov = new ffmpeg_movie($movie);
    ff_print_method_result($mov, 'getFileName');
    ff_print_method_result($mov, 'getFrameCount');
    ff_print_method_result($mov, 'getFrameRate');
    ff_print_method_result($mov, 'getComment');
    ff_print_method_result($mov, 'getTitle');
    ff_print_method_result($mov, 'getAuthor');
    ff_print_method_result($mov, 'getCopyright');
    ff_print_method_result($mov, 'getBitRate');
    ff_print_method_result($mov, 'hasAudio');
    ff_print_method_result($mov, 'getAudioStreamId');
    ff_print_method_result($mov, 'getAudioCodec');
    ff_print_method_result($mov, 'getAudioBitRate');
    ff_print_method_result($mov, 'getAudioSampleRate');
    ff_print_method_result($mov, 'getAudioChannels');
    ff_print_method_result($mov, 'hasVideo');
    ff_print_method_result($mov, 'getFrameHeight');
    ff_print_method_result($mov, 'getFrameWidth');
    ff_print_method_result($mov, 'getVideoStreamId');
    ff_print_method_result($mov, 'getVideoCodec');
    ff_print_method_result($mov, 'getVideoBitRate');
    ff_print_method_result($mov, 'getPixelFormat');
    ff_print_method_result($mov, 'getPixelAspectRatio');
    ff_print_method_result($mov, 'getFrameNumber');
    $frame = ff_print_method_result($mov, 'getFrame', 10);
    ff_print_method_result($mov, 'getFrameNumber');
    ff_print_method_result($frame, 'getWidth');
    ff_print_method_result($frame, 'getHeight');
echo "\n--------------------\n\n";
}

if (php_sapi_name() != 'cli') {
    echo '</pre>';
}


/* FUNCTIONS */
function ff_print_method_result($object, $method, $arg='') {
    $result = @$object->$method($arg);
    if ($result) {
        $p_result = is_object($result) ?  get_class($result) : $result;
        printf("%s->%s(%s) = %s\n", get_class($object), $method, $arg, $p_result);
    }
    return $result;
}

function print_class_methods($class) {
    echo "\nMethods available in class '$class':\n";
    $methods = get_class_methods($class);
    if (is_array($methods)) {
        foreach($methods as $method) {
            echo $method . "\n";
        }
    } else {
        echo "No Methods Defined\n";
        return array();
    }
    return $methods;
}


function getDirFiles($dirPath)
{
    if ($handle = opendir($dirPath))
    {
        while (false !== ($file = readdir($handle))) {
            $fullpath = $dirPath . '/' . $file;
            if (!is_dir($fullpath) && $file != "CVS" && $file != "." && $file != "..")
                $filesArr[] = trim($fullpath);
        }
        closedir($handle);
    } 

    return $filesArr;   
}

?>
