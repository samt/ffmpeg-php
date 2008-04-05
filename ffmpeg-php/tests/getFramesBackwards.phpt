--TEST--
ffmpeg getFrame backwards test
--SKIPIF--
<?php 
extension_loaded('ffmpeg') or die("skip ffmpeg extension not loaded"); 
extension_loaded('gd') or die("skip gd extension not avaliable.");
function_exists("imagecreatetruecolor") or die("skip function imagecreatetruecolor unavailable");
?>
--FILE--
<?php
$mov = new ffmpeg_movie(dirname(__FILE__) . '/test_media/test.avi');

$framecount = $mov->getFrameCount();
for($i = $framecount; $i > 0; $i--) {
    $img = sprintf("%s/test-%04d.png", dirname(__FILE__), $i);
    $frame = $mov->getFrame($i);
    $frame->resize(96, 120);
    $image = $frame->toGDImage();
    imagepng($image, $img);
    printf("ffmpeg getFramesBackward($i): md5 = %s\n", md5(file_get_contents($img)));
    imagedestroy($image);
    unlink($img);
}
?>
--EXPECT--
ffmpeg getFramesBackward(120): md5 = a8c910c28d20f8935ad46f7fcde9d8bb
ffmpeg getFramesBackward(119): md5 = f9a4b86d0eced6b38d0b6a343ec30bae
ffmpeg getFramesBackward(118): md5 = f1aa8ecadd7e09f1a1904844626427c9
ffmpeg getFramesBackward(117): md5 = 5675a7547c02da150f9800da32613660
ffmpeg getFramesBackward(116): md5 = 87f07be440736eb3db0d1d7325b5fc31
ffmpeg getFramesBackward(115): md5 = cb48a1599dee09ddd87630f02985c42e
ffmpeg getFramesBackward(114): md5 = 5ff9c460339f4c9c2a4e88f06f067d4a
ffmpeg getFramesBackward(113): md5 = b151da5278021c79c48e0c0e7d5f20b0
ffmpeg getFramesBackward(112): md5 = cec8ce44633074e2fd91e7889f25c6bc
ffmpeg getFramesBackward(111): md5 = 463fb7e7290f4e7277d359ad6616685c
ffmpeg getFramesBackward(110): md5 = 8287ee7e515845a7b7f0304f24853641
ffmpeg getFramesBackward(109): md5 = 8287ee7e515845a7b7f0304f24853641
ffmpeg getFramesBackward(108): md5 = e1b7f9e121d4fbf90eb45a0b6ac7dc0c
ffmpeg getFramesBackward(107): md5 = 7d227970984ce03ab9e53785e3374ad2
ffmpeg getFramesBackward(106): md5 = 64889fbebd0f8146ab7d0ff2c128bfd5
ffmpeg getFramesBackward(105): md5 = 816b8e76e10ece69dac1d77f8fab2c4d
ffmpeg getFramesBackward(104): md5 = e1013839b6108cd06697d589c0984d98
ffmpeg getFramesBackward(103): md5 = 1bcde3e32244c7152efadf8b1fcdd60f
ffmpeg getFramesBackward(102): md5 = fd068f928cf04344243c93a5f6e93178
ffmpeg getFramesBackward(101): md5 = a2bc99b405f2fe1a72060bd3391343b3
ffmpeg getFramesBackward(100): md5 = 95fbb1d4fa875922f0f01247e43cea6b
ffmpeg getFramesBackward(99): md5 = a346b6c8bf423dc9c50c9f84bc6c88e9
ffmpeg getFramesBackward(98): md5 = 77a677696d83d562df364be36c26b250
ffmpeg getFramesBackward(97): md5 = 77a677696d83d562df364be36c26b250
ffmpeg getFramesBackward(96): md5 = 80a17378c8825cff1edb799e677c15d5
ffmpeg getFramesBackward(95): md5 = bc10392a69195ce02bb7e5eae72964e2
ffmpeg getFramesBackward(94): md5 = f2c0d9f6329444c2dcb7d5255f3cacc4
ffmpeg getFramesBackward(93): md5 = 14da98f299f063a89a36f741f9f97b6c
ffmpeg getFramesBackward(92): md5 = 633d85e0f368507f6ccf238bd300d901
ffmpeg getFramesBackward(91): md5 = 714af6b3a44c2dff6d27679408a57d9a
ffmpeg getFramesBackward(90): md5 = 230ebad64a5554f1351d286b3abd3e10
ffmpeg getFramesBackward(89): md5 = 16cb8a015ab2faab37db3ca4bb9ef042
ffmpeg getFramesBackward(88): md5 = 056b7689edb5bb07ef369a93cd7626a3
ffmpeg getFramesBackward(87): md5 = 7e17d335753719a5205533a6312bc721
ffmpeg getFramesBackward(86): md5 = 8bc57eb36375d5aaf18b166cb71be5de
ffmpeg getFramesBackward(85): md5 = 8bc57eb36375d5aaf18b166cb71be5de
ffmpeg getFramesBackward(84): md5 = e90f7eb3cad4cfc13c2a00bb7993c981
ffmpeg getFramesBackward(83): md5 = 18504f8477b4faad3978a0cab4932a96
ffmpeg getFramesBackward(82): md5 = c2e6ea2e2787b996c29ae57ddf82dd13
ffmpeg getFramesBackward(81): md5 = 6f4233ba97586dbf494f3b876e71f186
ffmpeg getFramesBackward(80): md5 = 8dde407599ce8b5279cc433f1b2aa7af
ffmpeg getFramesBackward(79): md5 = 562376d8917f2deea794867e7badb913
ffmpeg getFramesBackward(78): md5 = 28714a576bdb1baaba9b628b15bc5c62
ffmpeg getFramesBackward(77): md5 = 8a2155b17e455a5061b148243a94eb3e
ffmpeg getFramesBackward(76): md5 = 943aacba174b369c4443b401fc440a95
ffmpeg getFramesBackward(75): md5 = 98d1e46abac89b05b03bf651993259cc
ffmpeg getFramesBackward(74): md5 = d5e7c72c7388fb1d257bdab6a69335f0
ffmpeg getFramesBackward(73): md5 = d5e7c72c7388fb1d257bdab6a69335f0
ffmpeg getFramesBackward(72): md5 = adf21c2c4326a20dedf065598527e784
ffmpeg getFramesBackward(71): md5 = 1c68925b12d780e016b27ee151c26412
ffmpeg getFramesBackward(70): md5 = c02eba91732832a088692e9b3ff61a25
ffmpeg getFramesBackward(69): md5 = 51f95110e451a790acf6d1f30722fe85
ffmpeg getFramesBackward(68): md5 = 9f0307d0476ff030a41957f59f3c6ece
ffmpeg getFramesBackward(67): md5 = 201418570db570c3210adac7b31fd232
ffmpeg getFramesBackward(66): md5 = dc36fd2ae2ee5994cbbe7d03bf3da6fb
ffmpeg getFramesBackward(65): md5 = 8ae21e839e717c3f2f6c56c36923a02e
ffmpeg getFramesBackward(64): md5 = 421d323a641d5427abf3e3c697909b9b
ffmpeg getFramesBackward(63): md5 = b11fba6f0ee021e861451112169f6576
ffmpeg getFramesBackward(62): md5 = 61c92c6b0a6f5e3fac54933ba275bef1
ffmpeg getFramesBackward(61): md5 = 61c92c6b0a6f5e3fac54933ba275bef1
ffmpeg getFramesBackward(60): md5 = 3ad0aea3ec0ed4903c91bc40fae62dfe
ffmpeg getFramesBackward(59): md5 = aa0d4502f98df33ba6f4c6bbd6e8f862
ffmpeg getFramesBackward(58): md5 = c6ecb3fad816ad3125d4b86b1b60978f
ffmpeg getFramesBackward(57): md5 = 1e60023c31afed379cd0a56815ca76b0
ffmpeg getFramesBackward(56): md5 = c9160acadc657dca5430de2cd3869f1c
ffmpeg getFramesBackward(55): md5 = 083fccf8dacb94244063b9dd0afddbec
ffmpeg getFramesBackward(54): md5 = 7b91bebdd6aa7fb16ea7ab540bb03f35
ffmpeg getFramesBackward(53): md5 = 5aaa761897fb4c5bb1ea23fc6cbd47fd
ffmpeg getFramesBackward(52): md5 = fe5a0942993f077ba693d9257a9355a9
ffmpeg getFramesBackward(51): md5 = b8c038b7e867fee513b7e26f7e166f1e
ffmpeg getFramesBackward(50): md5 = 3655bb01d46cf16eb68a8e809a6a8a64
ffmpeg getFramesBackward(49): md5 = 3655bb01d46cf16eb68a8e809a6a8a64
ffmpeg getFramesBackward(48): md5 = d00107805196133ddffd274851307b4d
ffmpeg getFramesBackward(47): md5 = 5adf4c8751f2e49ddec1f9a63fd7f844
ffmpeg getFramesBackward(46): md5 = ae20dc68b9596117eabcf6c66c66020e
ffmpeg getFramesBackward(45): md5 = 7d8cbfa20f64643c25b142d9d852a042
ffmpeg getFramesBackward(44): md5 = 9846be27852b03908760c580552c2a8f
ffmpeg getFramesBackward(43): md5 = 993987daed2467251a1e4e6070572132
ffmpeg getFramesBackward(42): md5 = a80febded850dd9d51292b8c69b7b30c
ffmpeg getFramesBackward(41): md5 = 85008c2d43ff9a90d27794db8f8b9077
ffmpeg getFramesBackward(40): md5 = 9999e82ac1c5a0ec8557108b688eb94d
ffmpeg getFramesBackward(39): md5 = f0b4c4acc1b3d082e46d196183399ae3
ffmpeg getFramesBackward(38): md5 = 080c53f570ed8f7d4c964126853a7f3d
ffmpeg getFramesBackward(37): md5 = 080c53f570ed8f7d4c964126853a7f3d
ffmpeg getFramesBackward(36): md5 = 77c8b386336392ced72bdca51bf4b08c
ffmpeg getFramesBackward(35): md5 = 9d55767739334aaded244749b0442c25
ffmpeg getFramesBackward(34): md5 = d30e148908701204edb1470b1bfc8e74
ffmpeg getFramesBackward(33): md5 = 0847cf99e4d19597c4a86dd269f38ac0
ffmpeg getFramesBackward(32): md5 = 2816261b40b5fa5eb6e5be7b98796c98
ffmpeg getFramesBackward(31): md5 = 883357ff579de7b39ec6cc73e6852202
ffmpeg getFramesBackward(30): md5 = e820eb937535aa77d040bd0ec665bc34
ffmpeg getFramesBackward(29): md5 = 9b5e0b55f5e4d78442b29c2016d37eca
ffmpeg getFramesBackward(28): md5 = c9683ada223f28e76fe9485528f28696
ffmpeg getFramesBackward(27): md5 = 0c59cc7b27db7fad0606c11a4afd46cc
ffmpeg getFramesBackward(26): md5 = 6495084488218320bbcfc01874e88227
ffmpeg getFramesBackward(25): md5 = 6495084488218320bbcfc01874e88227
ffmpeg getFramesBackward(24): md5 = 3f6851f899e4ef0977f8ef0fc9147b10
ffmpeg getFramesBackward(23): md5 = ee4cc27b47e8d7b7502581b310259985
ffmpeg getFramesBackward(22): md5 = ca39ebe5fdef5d7b3699912e753bf25d
ffmpeg getFramesBackward(21): md5 = da7af683679c34431e757aa893fc0663
ffmpeg getFramesBackward(20): md5 = bc72da843e3ec6b16d36e8f854cf2f33
ffmpeg getFramesBackward(19): md5 = bc72da843e3ec6b16d36e8f854cf2f33
ffmpeg getFramesBackward(18): md5 = fc6443025783f4f73dcc4b78285d13ea
ffmpeg getFramesBackward(17): md5 = 22b99b6f44b48212d4a9e98d25f2d54d
ffmpeg getFramesBackward(16): md5 = a8aacab4a8d72df3bddef1eb01398ed4
ffmpeg getFramesBackward(15): md5 = f0e97f999e4bc1089f4ad7b20fc23d01
ffmpeg getFramesBackward(14): md5 = 74569308fc3bc6fd4bd5b87246e9c675
ffmpeg getFramesBackward(13): md5 = 74569308fc3bc6fd4bd5b87246e9c675
ffmpeg getFramesBackward(12): md5 = 12734078a3cc1b48e405b0fd58d36027
ffmpeg getFramesBackward(11): md5 = a1d75529f0f82dba9f74600a11ddcb6b
ffmpeg getFramesBackward(10): md5 = 419f696ca7d22d25106dd551ff599922
ffmpeg getFramesBackward(9): md5 = 1f5bdd90e582c8fdd9edc50c31c4f918
ffmpeg getFramesBackward(8): md5 = 698acd175570dab75b8d4b2761d56a8f
ffmpeg getFramesBackward(7): md5 = a26bcf70f1f68d735ab474f67a236962
ffmpeg getFramesBackward(6): md5 = fc9e21ca7dcd44660124f9b1a8c6f226
ffmpeg getFramesBackward(5): md5 = b1978eeb20aa4eb52eb40e44e5ce0481
ffmpeg getFramesBackward(4): md5 = dff8ba2298d2c2bf295bfec166a8d1a5
ffmpeg getFramesBackward(3): md5 = dff8ba2298d2c2bf295bfec166a8d1a5
ffmpeg getFramesBackward(2): md5 = 8d2f758023720edd66da7661407337ac
ffmpeg getFramesBackward(1): md5 = c89bbcc8321b0002bcb5a16bceb61934
