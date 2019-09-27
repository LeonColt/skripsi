-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 06, 2017 at 05:04 AM
-- Server version: 5.7.14
-- PHP Version: 7.0.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `seraph`
--

-- --------------------------------------------------------

--
-- Table structure for table `app_list_winapi`
--

CREATE TABLE `app_list_winapi` (
  `app_id` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `app_key` varchar(25) NOT NULL,
  `owner` bigint(20) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `app_list_winapi`
--

INSERT INTO `app_list_winapi` (`app_id`, `app_key`, `owner`) VALUES
(00000000000000000001, '64WINY1385Y1IN3UUB9J6L5H4', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `benutzer`
--

CREATE TABLE `benutzer` (
  `id` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `username` varchar(50) NOT NULL,
  `email` varchar(75) NOT NULL,
  `pass` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `benutzer`
--

INSERT INTO `benutzer` (`id`, `username`, `email`, `pass`) VALUES
(00000000000000000001, 'lionmanbloodsucker', 'lionmanbloodsucker@yahoo.com', '$2y$10$YknHI7nNGSn6.0y9oJb4Fu8BNeulyf04EP/5kTxiUHfLOMAG1O5r.'),
(00000000232323232323, 'test', 'test@test.com', '$2y$10$ckOa4TwmfkVsoBmud6ayw./S.8XWSi4qbLsyfkBF7TCG.qLX8Z3By'),
(05893118706128225441, 'test1', 'test1@test1.com', '$2y$10$fRtbQVhfNTKj2bWIGRkV/OqwUmJj4eQlvmj8CEQMOIPYJtaYvAJxC');

-- --------------------------------------------------------

--
-- Table structure for table `download_record`
--

CREATE TABLE `download_record` (
  `id` varchar(255) NOT NULL,
  `benutzer` bigint(20) UNSIGNED DEFAULT NULL,
  `download_start` bigint(20) UNSIGNED NOT NULL,
  `download_finish` bigint(20) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `download_record_files`
--

CREATE TABLE `download_record_files` (
  `download_id` varchar(255) NOT NULL,
  `file_id` bigint(20) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `file`
--

CREATE TABLE `file` (
  `id` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(100) NOT NULL,
  `is_directory` tinyint(1) NOT NULL DEFAULT '0',
  `seen_by_all` tinyint(1) NOT NULL DEFAULT '0',
  `download_by_all` tinyint(1) NOT NULL DEFAULT '0',
  `modify_by_all` tinyint(1) NOT NULL DEFAULT '0',
  `owner` bigint(20) UNSIGNED ZEROFILL DEFAULT NULL,
  `parent` bigint(20) UNSIGNED DEFAULT NULL,
  `date_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `file`
--

INSERT INTO `file` (`id`, `name`, `is_directory`, `seen_by_all`, `download_by_all`, `modify_by_all`, `owner`, `parent`, `date_created`) VALUES
(00000000000000000001, 'Document', 1, 0, 0, 0, NULL, NULL, '2016-12-21 04:03:28'),
(00000000000000000002, 'PDF', 1, 0, 0, 0, NULL, 1, '2016-12-21 04:03:28'),
(04547994526835741156, 'test.zip', 0, 0, 1, 1, NULL, 2, '2017-02-21 08:23:48');

--
-- Triggers `file`
--
DELIMITER $$
CREATE TRIGGER `trigger_after_delete_file` AFTER DELETE ON `file` FOR EACH ROW INSERT INTO file_change_recorder(id_dir, owner) VALUES(old.parent, old.owner)
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `trigger_after_insert_file` AFTER INSERT ON `file` FOR EACH ROW INSERT INTO file_change_recorder(id_dir, owner) VALUES(NEW.parent, NEW.owner)
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `trigger_after_update_file` AFTER UPDATE ON `file` FOR EACH ROW BEGIN
INSERT INTO file_change_recorder(id_dir, owner) VALUES(old.parent, old.owner);
INSERT INTO file_change_recorder(id_dir, owner) VALUES (new.parent, new.owner);
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `file_change_recorder`
--

CREATE TABLE `file_change_recorder` (
  `waktu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `id_dir` bigint(20) UNSIGNED DEFAULT NULL,
  `owner` bigint(20) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `file_operation_log`
--

CREATE TABLE `file_operation_log` (
  `id_operation` varchar(100) NOT NULL,
  `id_dir` bigint(20) UNSIGNED NOT NULL,
  `owner` bigint(20) UNSIGNED DEFAULT NULL,
  `waktu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `file_operation_update_log`
--

CREATE TABLE `file_operation_update_log` (
  `id_operation` varchar(100) NOT NULL,
  `nama` varchar(100) NOT NULL,
  `format` mediumint(8) UNSIGNED NOT NULL,
  `size` bigint(20) UNSIGNED NOT NULL,
  `seen_by_all` tinyint(1) NOT NULL,
  `download_by_all` tinyint(1) NOT NULL,
  `modify_by_all` tinyint(1) NOT NULL,
  `owner` bigint(20) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `login_log_winapi`
--

CREATE TABLE `login_log_winapi` (
  `login_id` varchar(255) NOT NULL,
  `benutzer` bigint(20) UNSIGNED NOT NULL,
  `app` bigint(20) UNSIGNED NOT NULL,
  `waktu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `login_log_winapi`
--

INSERT INTO `login_log_winapi` (`login_id`, `benutzer`, `app`, `waktu`) VALUES
('7F8FFK2Z1WNLJKY3CA1U0KUVSEMG', 1, 1, '2017-04-03 14:31:07'),
('HRQ049XWQL3N0M2ZNCRZ3NJS6E5O', 1, 1, '2017-04-03 14:40:34'),
('PUNQFF8MN8UULVXJWNQ1AA3RV53H', 1, 1, '2017-04-03 14:28:10'),
('RBDIMFV048CV4ETBOPNRMHJ1F1B3', 1, 1, '2017-04-03 14:20:23');

-- --------------------------------------------------------

--
-- Table structure for table `login_log_winapi_fail`
--

CREATE TABLE `login_log_winapi_fail` (
  `waktu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `benutzer` bigint(20) UNSIGNED NOT NULL,
  `app` bigint(20) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `privilege_benutzer`
--

CREATE TABLE `privilege_benutzer` (
  `benutzer` bigint(20) UNSIGNED NOT NULL,
  `privilege_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `privilege_benutzer_on_object`
--

CREATE TABLE `privilege_benutzer_on_object` (
  `benutzer` bigint(20) UNSIGNED NOT NULL,
  `file_id` bigint(20) UNSIGNED NOT NULL,
  `privilege_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `privilege_group`
--

CREATE TABLE `privilege_group` (
  `privilege_group_id` int(10) UNSIGNED ZEROFILL NOT NULL,
  `group_name` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `privilege_group`
--

INSERT INTO `privilege_group` (`privilege_group_id`, `group_name`) VALUES
(0000000001, 'Super Admin'),
(0000000002, 'Admin'),
(0000000003, 'Member');

-- --------------------------------------------------------

--
-- Table structure for table `privilege_group_member`
--

CREATE TABLE `privilege_group_member` (
  `group_id` int(10) UNSIGNED NOT NULL,
  `benutzer` bigint(20) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `privilege_group_member`
--

INSERT INTO `privilege_group_member` (`group_id`, `benutzer`) VALUES
(1, 1),
(3, 232323232323),
(3, 5893118706128225441);

-- --------------------------------------------------------

--
-- Table structure for table `privilege_group_privilege`
--

CREATE TABLE `privilege_group_privilege` (
  `privilege_group_id` int(10) UNSIGNED NOT NULL,
  `privilege_id` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `recyle_bin`
--

CREATE TABLE `recyle_bin` (
  `id` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `file_name` varchar(100) NOT NULL,
  `is_directory` tinyint(1) NOT NULL DEFAULT '0',
  `file_owner` bigint(20) UNSIGNED DEFAULT NULL,
  `parent` bigint(20) UNSIGNED DEFAULT NULL,
  `parent_undeleted` bigint(20) UNSIGNED DEFAULT NULL,
  `date_deleted` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `original` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `seraph_privilege`
--

CREATE TABLE `seraph_privilege` (
  `privilege_id` int(10) UNSIGNED ZEROFILL NOT NULL,
  `privilege` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `seraph_privilege`
--

INSERT INTO `seraph_privilege` (`privilege_id`, `privilege`) VALUES
(0000000001, 'login'),
(0000000002, 'Login as Admin'),
(0000000003, 'Download Any File'),
(0000000004, 'Modify Any File'),
(0000000005, 'Delete Any File'),
(0000000006, 'Delete Member'),
(0000000007, 'Delete Admin'),
(0000000008, 'Join Group'),
(0000000009, 'Remove Member From Group'),
(0000000010, 'BlackList Member from Group'),
(0000000011, 'Kick Member From Group'),
(0000000012, 'Delete Group'),
(0000000013, 'Delete Any Group');

-- --------------------------------------------------------

--
-- Table structure for table `seraph_privilege_on_object`
--

CREATE TABLE `seraph_privilege_on_object` (
  `privilege_id` int(10) UNSIGNED NOT NULL,
  `privilage_name` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `seraph_privilege_on_object`
--

INSERT INTO `seraph_privilege_on_object` (`privilege_id`, `privilage_name`) VALUES
(1, 'Seen'),
(2, 'Download'),
(3, 'Upload On Directory'),
(4, 'Delete');

-- --------------------------------------------------------

--
-- Table structure for table `upload_record`
--

CREATE TABLE `upload_record` (
  `upload_id` varchar(25) NOT NULL,
  `parent` bigint(20) UNSIGNED DEFAULT NULL,
  `benutzer` bigint(20) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `upload_record_files`
--

CREATE TABLE `upload_record_files` (
  `file_id` bigint(20) UNSIGNED NOT NULL,
  `upload_id` varchar(25) NOT NULL,
  `name` varchar(100) NOT NULL,
  `is_directory` tinyint(1) NOT NULL DEFAULT '0',
  `size` bigint(20) UNSIGNED NOT NULL,
  `parent` bigint(20) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `app_list_winapi`
--
ALTER TABLE `app_list_winapi`
  ADD PRIMARY KEY (`app_id`),
  ADD KEY `owner` (`owner`);

--
-- Indexes for table `benutzer`
--
ALTER TABLE `benutzer`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`),
  ADD UNIQUE KEY `email` (`email`);

--
-- Indexes for table `download_record`
--
ALTER TABLE `download_record`
  ADD PRIMARY KEY (`id`),
  ADD KEY `downloader` (`benutzer`);

--
-- Indexes for table `download_record_files`
--
ALTER TABLE `download_record_files`
  ADD KEY `download_id` (`download_id`),
  ADD KEY `file_id` (`file_id`);

--
-- Indexes for table `file`
--
ALTER TABLE `file`
  ADD PRIMARY KEY (`id`),
  ADD KEY `owner` (`owner`),
  ADD KEY `parent` (`parent`);

--
-- Indexes for table `file_change_recorder`
--
ALTER TABLE `file_change_recorder`
  ADD KEY `id_dir` (`id_dir`),
  ADD KEY `owner` (`owner`);

--
-- Indexes for table `file_operation_log`
--
ALTER TABLE `file_operation_log`
  ADD PRIMARY KEY (`id_operation`),
  ADD KEY `id_file` (`id_dir`);

--
-- Indexes for table `file_operation_update_log`
--
ALTER TABLE `file_operation_update_log`
  ADD PRIMARY KEY (`id_operation`),
  ADD KEY `format` (`format`),
  ADD KEY `owner` (`owner`);

--
-- Indexes for table `login_log_winapi`
--
ALTER TABLE `login_log_winapi`
  ADD PRIMARY KEY (`login_id`),
  ADD KEY `benutzer` (`benutzer`),
  ADD KEY `app` (`app`);

--
-- Indexes for table `login_log_winapi_fail`
--
ALTER TABLE `login_log_winapi_fail`
  ADD KEY `benutzer` (`benutzer`),
  ADD KEY `app` (`app`);

--
-- Indexes for table `privilege_benutzer`
--
ALTER TABLE `privilege_benutzer`
  ADD PRIMARY KEY (`benutzer`,`privilege_id`),
  ADD KEY `privilege_id` (`privilege_id`);

--
-- Indexes for table `privilege_benutzer_on_object`
--
ALTER TABLE `privilege_benutzer_on_object`
  ADD PRIMARY KEY (`benutzer`,`file_id`,`privilege_id`),
  ADD KEY `file_id` (`file_id`),
  ADD KEY `privilege_id` (`privilege_id`);

--
-- Indexes for table `privilege_group`
--
ALTER TABLE `privilege_group`
  ADD PRIMARY KEY (`privilege_group_id`);

--
-- Indexes for table `privilege_group_member`
--
ALTER TABLE `privilege_group_member`
  ADD PRIMARY KEY (`group_id`,`benutzer`),
  ADD KEY `benutzer` (`benutzer`);

--
-- Indexes for table `privilege_group_privilege`
--
ALTER TABLE `privilege_group_privilege`
  ADD PRIMARY KEY (`privilege_group_id`,`privilege_id`),
  ADD KEY `privilege_id` (`privilege_id`);

--
-- Indexes for table `recyle_bin`
--
ALTER TABLE `recyle_bin`
  ADD PRIMARY KEY (`id`),
  ADD KEY `file_owner` (`file_owner`),
  ADD KEY `parent` (`parent`),
  ADD KEY `parent_undeleted` (`parent_undeleted`);

--
-- Indexes for table `seraph_privilege`
--
ALTER TABLE `seraph_privilege`
  ADD PRIMARY KEY (`privilege_id`);

--
-- Indexes for table `seraph_privilege_on_object`
--
ALTER TABLE `seraph_privilege_on_object`
  ADD PRIMARY KEY (`privilege_id`);

--
-- Indexes for table `upload_record`
--
ALTER TABLE `upload_record`
  ADD PRIMARY KEY (`upload_id`),
  ADD KEY `parent` (`parent`),
  ADD KEY `benutzer` (`benutzer`);

--
-- Indexes for table `upload_record_files`
--
ALTER TABLE `upload_record_files`
  ADD PRIMARY KEY (`file_id`),
  ADD KEY `upload_id` (`upload_id`),
  ADD KEY `parent` (`parent`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `app_list_winapi`
--
ALTER TABLE `app_list_winapi`
  MODIFY `app_id` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `seraph_privilege`
--
ALTER TABLE `seraph_privilege`
  MODIFY `privilege_id` int(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `app_list_winapi`
--
ALTER TABLE `app_list_winapi`
  ADD CONSTRAINT `app_list_winapi_ibfk_1` FOREIGN KEY (`owner`) REFERENCES `benutzer` (`id`);

--
-- Constraints for table `download_record`
--
ALTER TABLE `download_record`
  ADD CONSTRAINT `download_record_ibfk_2` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`);

--
-- Constraints for table `download_record_files`
--
ALTER TABLE `download_record_files`
  ADD CONSTRAINT `download_record_files_ibfk_1` FOREIGN KEY (`download_id`) REFERENCES `download_record` (`id`),
  ADD CONSTRAINT `download_record_files_ibfk_2` FOREIGN KEY (`file_id`) REFERENCES `file` (`id`);

--
-- Constraints for table `file`
--
ALTER TABLE `file`
  ADD CONSTRAINT `file_ibfk_2` FOREIGN KEY (`owner`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `file_ibfk_3` FOREIGN KEY (`parent`) REFERENCES `file` (`id`);

--
-- Constraints for table `file_operation_update_log`
--
ALTER TABLE `file_operation_update_log`
  ADD CONSTRAINT `file_operation_update_log_ibfk_3` FOREIGN KEY (`owner`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `file_operation_update_log_ibfk_4` FOREIGN KEY (`id_operation`) REFERENCES `file_operation_log` (`id_operation`);

--
-- Constraints for table `login_log_winapi`
--
ALTER TABLE `login_log_winapi`
  ADD CONSTRAINT `login_log_winapi_ibfk_1` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `login_log_winapi_ibfk_2` FOREIGN KEY (`app`) REFERENCES `app_list_winapi` (`app_id`);

--
-- Constraints for table `login_log_winapi_fail`
--
ALTER TABLE `login_log_winapi_fail`
  ADD CONSTRAINT `login_log_winapi_fail_ibfk_1` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `login_log_winapi_fail_ibfk_2` FOREIGN KEY (`app`) REFERENCES `app_list_winapi` (`app_id`);

--
-- Constraints for table `privilege_benutzer`
--
ALTER TABLE `privilege_benutzer`
  ADD CONSTRAINT `privilege_benutzer_ibfk_1` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `privilege_benutzer_ibfk_2` FOREIGN KEY (`privilege_id`) REFERENCES `seraph_privilege` (`privilege_id`);

--
-- Constraints for table `privilege_benutzer_on_object`
--
ALTER TABLE `privilege_benutzer_on_object`
  ADD CONSTRAINT `privilege_benutzer_on_object_ibfk_1` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `privilege_benutzer_on_object_ibfk_2` FOREIGN KEY (`file_id`) REFERENCES `file` (`id`),
  ADD CONSTRAINT `privilege_benutzer_on_object_ibfk_3` FOREIGN KEY (`privilege_id`) REFERENCES `seraph_privilege_on_object` (`privilege_id`);

--
-- Constraints for table `privilege_group_member`
--
ALTER TABLE `privilege_group_member`
  ADD CONSTRAINT `privilege_group_member_ibfk_1` FOREIGN KEY (`group_id`) REFERENCES `privilege_group` (`privilege_group_id`),
  ADD CONSTRAINT `privilege_group_member_ibfk_2` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `privilege_group_member_ibfk_3` FOREIGN KEY (`group_id`) REFERENCES `privilege_group` (`privilege_group_id`),
  ADD CONSTRAINT `privilege_group_member_ibfk_4` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`);

--
-- Constraints for table `privilege_group_privilege`
--
ALTER TABLE `privilege_group_privilege`
  ADD CONSTRAINT `privilege_group_privilege_ibfk_1` FOREIGN KEY (`privilege_group_id`) REFERENCES `privilege_group` (`privilege_group_id`),
  ADD CONSTRAINT `privilege_group_privilege_ibfk_2` FOREIGN KEY (`privilege_id`) REFERENCES `seraph_privilege` (`privilege_id`);

--
-- Constraints for table `recyle_bin`
--
ALTER TABLE `recyle_bin`
  ADD CONSTRAINT `recyle_bin_ibfk_2` FOREIGN KEY (`file_owner`) REFERENCES `benutzer` (`id`),
  ADD CONSTRAINT `recyle_bin_ibfk_3` FOREIGN KEY (`parent`) REFERENCES `recyle_bin` (`id`),
  ADD CONSTRAINT `recyle_bin_ibfk_4` FOREIGN KEY (`parent_undeleted`) REFERENCES `file` (`id`);

--
-- Constraints for table `upload_record`
--
ALTER TABLE `upload_record`
  ADD CONSTRAINT `upload_record_ibfk_1` FOREIGN KEY (`parent`) REFERENCES `file` (`id`),
  ADD CONSTRAINT `upload_record_ibfk_2` FOREIGN KEY (`benutzer`) REFERENCES `benutzer` (`id`);

--
-- Constraints for table `upload_record_files`
--
ALTER TABLE `upload_record_files`
  ADD CONSTRAINT `upload_record_files_ibfk_1` FOREIGN KEY (`upload_id`) REFERENCES `upload_record` (`upload_id`),
  ADD CONSTRAINT `upload_record_files_ibfk_3` FOREIGN KEY (`parent`) REFERENCES `upload_record_files` (`file_id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
