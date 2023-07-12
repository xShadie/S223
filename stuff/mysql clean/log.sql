/*
Navicat MySQL Data Transfer

Source Server         : Windows Lokal
Source Server Version : 50650
Source Host           : localhost:3306
Source Database       : log

Target Server Type    : MYSQL
Target Server Version : 50650
File Encoding         : 65001

Date: 2022-10-04 15:35:30
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `boot`
-- ----------------------------
DROP TABLE IF EXISTS `boot`;
CREATE TABLE `boot` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime DEFAULT NULL,
  `hostname` varchar(50) COLLATE big5_bin DEFAULT NULL,
  `channel` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=big5 COLLATE=big5_bin;

-- ----------------------------
-- Records of boot
-- ----------------------------

-- ----------------------------
-- Table structure for `change_name`
-- ----------------------------
DROP TABLE IF EXISTS `change_name`;
CREATE TABLE `change_name` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pid` int(20) DEFAULT '0',
  `old_name` varchar(30) DEFAULT NULL,
  `new_name` varchar(30) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `ip` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of change_name
-- ----------------------------

-- ----------------------------
-- Table structure for `command`
-- ----------------------------
DROP TABLE IF EXISTS `command`;
CREATE TABLE `command` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `name` varchar(12) NOT NULL DEFAULT 'NONAME',
  `command` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of command
-- ----------------------------

-- ----------------------------
-- Table structure for `cube`
-- ----------------------------
DROP TABLE IF EXISTS `cube`;
CREATE TABLE `cube` (
  `id` int(20) NOT NULL AUTO_INCREMENT,
  `pid` int(11) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `item_vnum` int(40) DEFAULT NULL,
  `item_id` bigint(20) DEFAULT NULL,
  `item_count` int(11) DEFAULT NULL,
  `success` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of cube
-- ----------------------------

-- ----------------------------
-- Table structure for `exchange_gold`
-- ----------------------------
DROP TABLE IF EXISTS `exchange_gold`;
CREATE TABLE `exchange_gold` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime DEFAULT NULL,
  `from` varchar(255) COLLATE utf8mb4_bin NOT NULL DEFAULT '',
  `to` varchar(255) COLLATE utf8mb4_bin NOT NULL DEFAULT '',
  `gold` bigint(20) NOT NULL DEFAULT '0',
  `fromAccID` int(11) NOT NULL,
  `toAccID` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin ROW_FORMAT=FIXED;

-- ----------------------------
-- Records of exchange_gold
-- ----------------------------

-- ----------------------------
-- Table structure for `exchange_items`
-- ----------------------------
DROP TABLE IF EXISTS `exchange_items`;
CREATE TABLE `exchange_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL,
  `from` varchar(255) COLLATE utf8mb4_bin NOT NULL DEFAULT '',
  `to` varchar(255) CHARACTER SET big5 COLLATE big5_bin NOT NULL DEFAULT '',
  `itemname` varchar(255) COLLATE utf8mb4_bin NOT NULL DEFAULT '',
  `itemid` bigint(20) NOT NULL,
  `count` int(11) NOT NULL,
  `fromAccID` int(11) NOT NULL,
  `toAccID` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- ----------------------------
-- Records of exchange_items
-- ----------------------------

-- ----------------------------
-- Table structure for `hack`
-- ----------------------------
DROP TABLE IF EXISTS `hack`;
CREATE TABLE `hack` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime DEFAULT NULL,
  `accountid` int(11) DEFAULT NULL,
  `name` varchar(255) COLLATE big5_bin DEFAULT NULL,
  `why` varchar(255) COLLATE big5_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=big5 COLLATE=big5_bin;

-- ----------------------------
-- Records of hack
-- ----------------------------

-- ----------------------------
-- Table structure for `quest_reward`
-- ----------------------------
DROP TABLE IF EXISTS `quest_reward`;
CREATE TABLE `quest_reward` (
  `1` varchar(50) CHARACTER SET utf8mb4 DEFAULT NULL,
  `2` int(11) DEFAULT NULL,
  `3` int(11) DEFAULT NULL,
  `4` int(11) DEFAULT NULL,
  `5` int(11) DEFAULT NULL,
  `6` int(11) DEFAULT NULL,
  `7` time DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=big5 COLLATE=big5_bin;

-- ----------------------------
-- Records of quest_reward
-- ----------------------------

-- ----------------------------
-- Table structure for `refine`
-- ----------------------------
DROP TABLE IF EXISTS `refine`;
CREATE TABLE `refine` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime DEFAULT NULL,
  `name` varchar(255) COLLATE utf8mb4_bin DEFAULT NULL,
  `itemname` varchar(255) COLLATE utf8mb4_bin DEFAULT NULL,
  `itemid` int(11) unsigned DEFAULT NULL,
  `newitemid` int(11) DEFAULT NULL,
  `status` varchar(255) CHARACTER SET big5 COLLATE big5_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- ----------------------------
-- Records of refine
-- ----------------------------
