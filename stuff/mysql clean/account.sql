/*
Navicat MySQL Data Transfer

Source Server         : Windows Lokal
Source Server Version : 50650
Source Host           : localhost:3306
Source Database       : account

Target Server Type    : MYSQL
Target Server Version : 50650
File Encoding         : 65001

Date: 2022-10-04 15:35:41
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `account`
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(30) NOT NULL DEFAULT '',
  `password` varchar(45) NOT NULL DEFAULT '',
  `social_id` varchar(13) NOT NULL DEFAULT '',
  `email` varchar(64) NOT NULL DEFAULT '',
  `create_time` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `status` varchar(8) NOT NULL DEFAULT 'OK',
  `empire` tinyint(4) NOT NULL DEFAULT '0',
  `availDt` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `gold_expire` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `silver_expire` datetime NOT NULL DEFAULT '2035-05-05 21:00:00',
  `safebox_expire` datetime NOT NULL DEFAULT '2035-05-05 21:00:00',
  `autoloot_expire` datetime NOT NULL DEFAULT '2035-05-05 21:00:00',
  `fish_mind_expire` datetime NOT NULL DEFAULT '2035-05-05 21:00:00',
  `marriage_fast_expire` datetime NOT NULL DEFAULT '2035-05-05 21:00:00',
  `money_drop_rate_expire` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_play` datetime DEFAULT NULL,
  `coins` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `login` (`login`),
  KEY `social_id` (`social_id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=ascii;

-- ----------------------------
-- Records of account
-- ----------------------------
INSERT INTO `account` VALUES ('1', 'admin', '*23AE809DDACAF96AF0FD78ED04B6A265E05AA257', '1234567', '', '0000-00-00 00:00:00', 'OK', '0', '2021-03-12 17:48:32', '0000-00-00 00:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '0000-00-00 00:00:00', '2022-10-04 15:25:18', '0');
INSERT INTO `account` VALUES ('2', 'admin2', '*23AE809DDACAF96AF0FD78ED04B6A265E05AA257', '1234567', '', '0000-00-00 00:00:00', 'OK', '0', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '0000-00-00 00:00:00', '2022-09-20 07:34:03', '0');
INSERT INTO `account` VALUES ('3', 'admin3', '*23AE809DDACAF96AF0FD78ED04B6A265E05AA257', '1234567', '', '0000-00-00 00:00:00', 'OK', '0', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '2035-05-05 21:00:00', '0000-00-00 00:00:00', '2022-02-05 15:08:30', '0');
INSERT INTO `account` VALUES ('4', 'admin4', '*23AE809DDACAF96AF0FD78ED04B6A265E05AA257', '1234567', '', '0000-00-00 00:00:00', 'OK', '0', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '2022-01-06 14:42:22', '0');
