--TEST--
ffmpeg getFrameRate test
--SKIPIF--
<?php extension_loaded('ffmpeg') or die("ffmpeg extension not loaded"); ?>
--FILE--
<?php
$mov = new ffmpeg_movie(dirname(__FILE__) . '/test.avi');
printf("ffmpeg getFrameRate(): %0.2f\n", $mov->getFrameRate());
?>
--EXPECT--
ffmpeg getFrameRate(): 24.00
