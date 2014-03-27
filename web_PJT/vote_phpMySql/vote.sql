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
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of vote
-- ----------------------------
INSERT INTO vote VALUES ('24', '1', '功能', '1');
INSERT INTO vote VALUES ('25', '1', '颜色', '1');
INSERT INTO vote VALUES ('26', '1', '版面', '1');
INSERT INTO vote VALUES ('27', '1', '栏目', '1');

-- ----------------------------
-- Table structure for `votetitle`
-- ----------------------------
DROP TABLE IF EXISTS `votetitle`;
CREATE TABLE `votetitle` (
  `titleid` int(10) NOT NULL,
  `votetitle` varchar(50) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of votetitle
-- ----------------------------
INSERT INTO votetitle VALUES ('1', '您认为本网站还有那些要做调整？');
