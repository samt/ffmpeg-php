--TEST--
ffmpeg getFrameWidth test
--SKIPIF--
<?php extension_loaded('ffmpeg') or die("ffmpeg extension not loaded\n"); ?>
--FILE--
<?php
$mov = new ffmpeg_movie(dirname(__FILE__) . '/test.avi');
printf("ffmpeg getFrameWidth(): %d\n", $mov->getFrameWidth());
?>
--EXPECT--
ffmpeg getFrameWidth(): 190
