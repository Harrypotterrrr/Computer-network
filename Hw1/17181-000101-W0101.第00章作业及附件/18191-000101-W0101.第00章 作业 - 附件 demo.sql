drop database if exists demo;
create database demo;

use demo;

drop table if exists student;
create table student (
sno char(9) not null,
sname char(8) not null,
ssex char(2) not null default '男',
sage tinyint(2),
sdept char(2)
);

insert into student values('200215121','李勇','男',20,'CS');
insert into student values('200215122','刘晨','女',19,'CS');
insert into student values('200215123','王敏','女',18,'MA');
insert into student values('200215125','张立','男',19,'IS');
