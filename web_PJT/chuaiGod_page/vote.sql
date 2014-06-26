/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50091
Source Host           : localhost:3306
Source Database       : vote

Target Server Type    : MYSQL
Target Server Version : 50091
File Encoding         : 65001

Date: 2013-03-20 23:04:38
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `vote`
-- ----------------------------
DROP TABLE IF EXISTS `vote`;
CREATE TABLE `vote` (
  `id` int(10) NOT NULL auto_increment,
  `titleid` int(10) default NULL,
  `item` varchar(50) default NULL,
  `count` int(10) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of vote
-- ----------------------------
INSERT INTO vote VALUES ('1', '1', '功能', '0');
INSERT INTO vote VALUES ('2', '1', '颜色', '0');
INSERT INTO vote VALUES ('3', '1', '版面', '0');
INSERT INTO vote VALUES ('4', '1', '栏目', '0');

-- ----------------------------
-- Table structure for `votetitle`
-- ----------------------------
DROP TABLE IF EXISTS `votetitle`;
CREATE TABLE `votetitle` (
  `titleid` int(10) NOT NULL,
  `votetitle` varchar(50) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of votetitle
-- ----------------------------
INSERT INTO votetitle VALUES ('1', '您认为本网站还有那些要做调整？');

-- ----------------------------
-- Table structure for `voter_ip`
-- ----------------------------
DROP TABLE IF EXISTS `voter_ip`;
CREATE TABLE `voter_ip` (
  `id` int(10) default NULL auto_increment,
  `v_count` int(10) default NULL,
  `user_ip` varchar(50) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
