--TEST--
ffmpeg getPTS test (Fixme: This test is no good with robot.avi)
--SKIPIF--
<?php 
extension_loaded('ffmpeg') or die("skip ffmpeg extension not loaded"); 
extension_loaded('gd') or die("skip gd extension not avaliable.");
function_exists("imagecreatetruecolor") or die("skip function imagecreatetruecolor unavailable");
?>
--FILE--
<?php
$mov = new ffmpeg_movie(dirname(__FILE__) . '/test_media/robot.avi');

$framecount = $mov->getFrameCount();
for($i = 1; $i <= $framecount; $i++) {
    $frame = $mov->getFrame($i);
    printf("ffmpeg getPresentationTimestamp($i): = %s\n", $frame->getPTS());
}
?>
--EXPECT--
ffmpeg getPresentationTimestamp(1): = -9223372036850
ffmpeg getPresentationTimestamp(2): = -9223372036850
ffmpeg getPresentationTimestamp(3): = 0.0268
ffmpeg getPresentationTimestamp(4): = -9223372036850
ffmpeg getPresentationTimestamp(5): = -9223372036850
ffmpeg getPresentationTimestamp(6): = 0.0376
ffmpeg getPresentationTimestamp(7): = -9223372036850
ffmpeg getPresentationTimestamp(8): = -9223372036850
ffmpeg getPresentationTimestamp(9): = 0.0484
ffmpeg getPresentationTimestamp(10): = -9223372036850
ffmpeg getPresentationTimestamp(11): = -9223372036850
ffmpeg getPresentationTimestamp(12): = 0.0592
ffmpeg getPresentationTimestamp(13): = -9223372036850
ffmpeg getPresentationTimestamp(14): = -9223372036850
ffmpeg getPresentationTimestamp(15): = 0.07
ffmpeg getPresentationTimestamp(16): = -9223372036850
ffmpeg getPresentationTimestamp(17): = -9223372036850
ffmpeg getPresentationTimestamp(18): = 0.0808
ffmpeg getPresentationTimestamp(19): = -9223372036850
ffmpeg getPresentationTimestamp(20): = -9223372036850
ffmpeg getPresentationTimestamp(21): = 0.0916
ffmpeg getPresentationTimestamp(22): = -9223372036850
ffmpeg getPresentationTimestamp(23): = -9223372036850
ffmpeg getPresentationTimestamp(24): = 0.1024
ffmpeg getPresentationTimestamp(25): = -9223372036850
ffmpeg getPresentationTimestamp(26): = -9223372036850
ffmpeg getPresentationTimestamp(27): = 0.1132
ffmpeg getPresentationTimestamp(28): = -9223372036850
ffmpeg getPresentationTimestamp(29): = -9223372036850
ffmpeg getPresentationTimestamp(30): = 0.124
ffmpeg getPresentationTimestamp(31): = -9223372036850
ffmpeg getPresentationTimestamp(32): = -9223372036850
ffmpeg getPresentationTimestamp(33): = 0.1348
ffmpeg getPresentationTimestamp(34): = -9223372036850
ffmpeg getPresentationTimestamp(35): = -9223372036850
ffmpeg getPresentationTimestamp(36): = 0.1456
ffmpeg getPresentationTimestamp(37): = -9223372036850
ffmpeg getPresentationTimestamp(38): = -9223372036850
ffmpeg getPresentationTimestamp(39): = 0.1564
ffmpeg getPresentationTimestamp(40): = -9223372036850
ffmpeg getPresentationTimestamp(41): = -9223372036850
ffmpeg getPresentationTimestamp(42): = 0.1672
ffmpeg getPresentationTimestamp(43): = -9223372036850
ffmpeg getPresentationTimestamp(44): = -9223372036850
ffmpeg getPresentationTimestamp(45): = 0.178
ffmpeg getPresentationTimestamp(46): = -9223372036850
ffmpeg getPresentationTimestamp(47): = -9223372036850
ffmpeg getPresentationTimestamp(48): = 0.1888
ffmpeg getPresentationTimestamp(49): = -9223372036850
ffmpeg getPresentationTimestamp(50): = -9223372036850
ffmpeg getPresentationTimestamp(51): = 0.1996
ffmpeg getPresentationTimestamp(52): = -9223372036850
ffmpeg getPresentationTimestamp(53): = -9223372036850
ffmpeg getPresentationTimestamp(54): = 0.2104
ffmpeg getPresentationTimestamp(55): = -9223372036850
ffmpeg getPresentationTimestamp(56): = -9223372036850
ffmpeg getPresentationTimestamp(57): = 0.2212
ffmpeg getPresentationTimestamp(58): = -9223372036850
ffmpeg getPresentationTimestamp(59): = -9223372036850
ffmpeg getPresentationTimestamp(60): = 0.232
ffmpeg getPresentationTimestamp(61): = -9223372036850
ffmpeg getPresentationTimestamp(62): = -9223372036850
ffmpeg getPresentationTimestamp(63): = 0.2428
ffmpeg getPresentationTimestamp(64): = -9223372036850
ffmpeg getPresentationTimestamp(65): = -9223372036850
ffmpeg getPresentationTimestamp(66): = 0.2536
ffmpeg getPresentationTimestamp(67): = -9223372036850
ffmpeg getPresentationTimestamp(68): = -9223372036850
ffmpeg getPresentationTimestamp(69): = 0.2644
ffmpeg getPresentationTimestamp(70): = -9223372036850
ffmpeg getPresentationTimestamp(71): = -9223372036850
ffmpeg getPresentationTimestamp(72): = 0.2752
ffmpeg getPresentationTimestamp(73): = -9223372036850
ffmpeg getPresentationTimestamp(74): = -9223372036850
ffmpeg getPresentationTimestamp(75): = 0.286
ffmpeg getPresentationTimestamp(76): = -9223372036850
ffmpeg getPresentationTimestamp(77): = -9223372036850
ffmpeg getPresentationTimestamp(78): = 0.2968
ffmpeg getPresentationTimestamp(79): = -9223372036850
ffmpeg getPresentationTimestamp(80): = -9223372036850
ffmpeg getPresentationTimestamp(81): = 0.3076
ffmpeg getPresentationTimestamp(82): = -9223372036850
ffmpeg getPresentationTimestamp(83): = -9223372036850
ffmpeg getPresentationTimestamp(84): = 0.3184
ffmpeg getPresentationTimestamp(85): = -9223372036850
ffmpeg getPresentationTimestamp(86): = -9223372036850
ffmpeg getPresentationTimestamp(87): = 0.3292
ffmpeg getPresentationTimestamp(88): = -9223372036850
ffmpeg getPresentationTimestamp(89): = -9223372036850
ffmpeg getPresentationTimestamp(90): = 0.34
ffmpeg getPresentationTimestamp(91): = -9223372036850
ffmpeg getPresentationTimestamp(92): = -9223372036850
ffmpeg getPresentationTimestamp(93): = 0.3508
ffmpeg getPresentationTimestamp(94): = -9223372036850
ffmpeg getPresentationTimestamp(95): = -9223372036850
ffmpeg getPresentationTimestamp(96): = 0.3616
ffmpeg getPresentationTimestamp(97): = -9223372036850
ffmpeg getPresentationTimestamp(98): = -9223372036850
ffmpeg getPresentationTimestamp(99): = 0.3724
ffmpeg getPresentationTimestamp(100): = -9223372036850
ffmpeg getPresentationTimestamp(101): = -9223372036850
ffmpeg getPresentationTimestamp(102): = 0.3832
ffmpeg getPresentationTimestamp(103): = -9223372036850
ffmpeg getPresentationTimestamp(104): = -9223372036850
ffmpeg getPresentationTimestamp(105): = 0.394
ffmpeg getPresentationTimestamp(106): = -9223372036850
ffmpeg getPresentationTimestamp(107): = -9223372036850
ffmpeg getPresentationTimestamp(108): = 0.4048
ffmpeg getPresentationTimestamp(109): = -9223372036850
ffmpeg getPresentationTimestamp(110): = -9223372036850
ffmpeg getPresentationTimestamp(111): = 0.4156
ffmpeg getPresentationTimestamp(112): = -9223372036850
ffmpeg getPresentationTimestamp(113): = -9223372036850
ffmpeg getPresentationTimestamp(114): = 0.4264
ffmpeg getPresentationTimestamp(115): = -9223372036850
ffmpeg getPresentationTimestamp(116): = -9223372036850
ffmpeg getPresentationTimestamp(117): = 0.4372
ffmpeg getPresentationTimestamp(118): = -9223372036850
ffmpeg getPresentationTimestamp(119): = -9223372036850
ffmpeg getPresentationTimestamp(120): = 0.448
ffmpeg getPresentationTimestamp(121): = -9223372036850
ffmpeg getPresentationTimestamp(122): = -9223372036850
ffmpeg getPresentationTimestamp(123): = 0.4588
ffmpeg getPresentationTimestamp(124): = -9223372036850
ffmpeg getPresentationTimestamp(125): = -9223372036850
ffmpeg getPresentationTimestamp(126): = 0.4696
ffmpeg getPresentationTimestamp(127): = -9223372036850
ffmpeg getPresentationTimestamp(128): = -9223372036850
ffmpeg getPresentationTimestamp(129): = 0.4804
ffmpeg getPresentationTimestamp(130): = -9223372036850
ffmpeg getPresentationTimestamp(131): = -9223372036850
ffmpeg getPresentationTimestamp(132): = 0.4912
ffmpeg getPresentationTimestamp(133): = -9223372036850
ffmpeg getPresentationTimestamp(134): = -9223372036850
ffmpeg getPresentationTimestamp(135): = 0.502
ffmpeg getPresentationTimestamp(136): = -9223372036850
ffmpeg getPresentationTimestamp(137): = -9223372036850
ffmpeg getPresentationTimestamp(138): = 0.5128
ffmpeg getPresentationTimestamp(139): = -9223372036850
ffmpeg getPresentationTimestamp(140): = -9223372036850
ffmpeg getPresentationTimestamp(141): = 0.5236
ffmpeg getPresentationTimestamp(142): = -9223372036850
ffmpeg getPresentationTimestamp(143): = -9223372036850
ffmpeg getPresentationTimestamp(144): = 0.5344
ffmpeg getPresentationTimestamp(145): = -9223372036850
ffmpeg getPresentationTimestamp(146): = -9223372036850
ffmpeg getPresentationTimestamp(147): = 0.5452
ffmpeg getPresentationTimestamp(148): = -9223372036850
ffmpeg getPresentationTimestamp(149): = -9223372036850
ffmpeg getPresentationTimestamp(150): = 0.556
ffmpeg getPresentationTimestamp(151): = -9223372036850
ffmpeg getPresentationTimestamp(152): = -9223372036850
ffmpeg getPresentationTimestamp(153): = 0.5668
ffmpeg getPresentationTimestamp(154): = -9223372036850
ffmpeg getPresentationTimestamp(155): = -9223372036850
ffmpeg getPresentationTimestamp(156): = 0.5776
ffmpeg getPresentationTimestamp(157): = -9223372036850
ffmpeg getPresentationTimestamp(158): = -9223372036850
ffmpeg getPresentationTimestamp(159): = 0.5884
ffmpeg getPresentationTimestamp(160): = -9223372036850
ffmpeg getPresentationTimestamp(161): = -9223372036850
ffmpeg getPresentationTimestamp(162): = 0.5992
ffmpeg getPresentationTimestamp(163): = -9223372036850
ffmpeg getPresentationTimestamp(164): = -9223372036850
ffmpeg getPresentationTimestamp(165): = 0.61
ffmpeg getPresentationTimestamp(166): = -9223372036850
ffmpeg getPresentationTimestamp(167): = -9223372036850
ffmpeg getPresentationTimestamp(168): = 0.6208
ffmpeg getPresentationTimestamp(169): = -9223372036850
ffmpeg getPresentationTimestamp(170): = -9223372036850
ffmpeg getPresentationTimestamp(171): = 0.6316
ffmpeg getPresentationTimestamp(172): = -9223372036850
ffmpeg getPresentationTimestamp(173): = -9223372036850
ffmpeg getPresentationTimestamp(174): = 0.6424
ffmpeg getPresentationTimestamp(175): = -9223372036850
ffmpeg getPresentationTimestamp(176): = -9223372036850
ffmpeg getPresentationTimestamp(177): = 0.6532
ffmpeg getPresentationTimestamp(178): = -9223372036850
ffmpeg getPresentationTimestamp(179): = -9223372036850
ffmpeg getPresentationTimestamp(180): = 0.664
ffmpeg getPresentationTimestamp(181): = -9223372036850
ffmpeg getPresentationTimestamp(182): = -9223372036850
ffmpeg getPresentationTimestamp(183): = 0.6748
ffmpeg getPresentationTimestamp(184): = -9223372036850
ffmpeg getPresentationTimestamp(185): = -9223372036850
ffmpeg getPresentationTimestamp(186): = 0.6856
ffmpeg getPresentationTimestamp(187): = -9223372036850
ffmpeg getPresentationTimestamp(188): = -9223372036850
ffmpeg getPresentationTimestamp(189): = 0.6964
ffmpeg getPresentationTimestamp(190): = -9223372036850
ffmpeg getPresentationTimestamp(191): = -9223372036850
ffmpeg getPresentationTimestamp(192): = 0.7072
ffmpeg getPresentationTimestamp(193): = -9223372036850
ffmpeg getPresentationTimestamp(194): = -9223372036850
ffmpeg getPresentationTimestamp(195): = 0.718
ffmpeg getPresentationTimestamp(196): = -9223372036850
ffmpeg getPresentationTimestamp(197): = -9223372036850
ffmpeg getPresentationTimestamp(198): = 0.7288
ffmpeg getPresentationTimestamp(199): = -9223372036850
ffmpeg getPresentationTimestamp(200): = -9223372036850
ffmpeg getPresentationTimestamp(201): = 0.7396
ffmpeg getPresentationTimestamp(202): = -9223372036850
ffmpeg getPresentationTimestamp(203): = -9223372036850
ffmpeg getPresentationTimestamp(204): = 0.7504
ffmpeg getPresentationTimestamp(205): = -9223372036850
ffmpeg getPresentationTimestamp(206): = -9223372036850
ffmpeg getPresentationTimestamp(207): = 0.7612
ffmpeg getPresentationTimestamp(208): = -9223372036850
ffmpeg getPresentationTimestamp(209): = -9223372036850
ffmpeg getPresentationTimestamp(210): = 0.772
ffmpeg getPresentationTimestamp(211): = -9223372036850
ffmpeg getPresentationTimestamp(212): = -9223372036850
ffmpeg getPresentationTimestamp(213): = 0.7828
ffmpeg getPresentationTimestamp(214): = -9223372036850
ffmpeg getPresentationTimestamp(215): = -9223372036850
ffmpeg getPresentationTimestamp(216): = 0.7936
ffmpeg getPresentationTimestamp(217): = -9223372036850
ffmpeg getPresentationTimestamp(218): = -9223372036850
ffmpeg getPresentationTimestamp(219): = 0.8044
ffmpeg getPresentationTimestamp(220): = -9223372036850
ffmpeg getPresentationTimestamp(221): = -9223372036850
ffmpeg getPresentationTimestamp(222): = 0.8152
ffmpeg getPresentationTimestamp(223): = -9223372036850
ffmpeg getPresentationTimestamp(224): = -9223372036850
ffmpeg getPresentationTimestamp(225): = 0.826
ffmpeg getPresentationTimestamp(226): = -9223372036850
ffmpeg getPresentationTimestamp(227): = -9223372036850
ffmpeg getPresentationTimestamp(228): = 0.8368
ffmpeg getPresentationTimestamp(229): = -9223372036850
ffmpeg getPresentationTimestamp(230): = -9223372036850
ffmpeg getPresentationTimestamp(231): = 0.8476
ffmpeg getPresentationTimestamp(232): = -9223372036850
ffmpeg getPresentationTimestamp(233): = -9223372036850
ffmpeg getPresentationTimestamp(234): = 0.8584
ffmpeg getPresentationTimestamp(235): = -9223372036850
ffmpeg getPresentationTimestamp(236): = -9223372036850
ffmpeg getPresentationTimestamp(237): = 0.8692
ffmpeg getPresentationTimestamp(238): = -9223372036850
ffmpeg getPresentationTimestamp(239): = -9223372036850
ffmpeg getPresentationTimestamp(240): = 0.88
