drop database if exists demo;
create database demo;

use demo;

drop table if exists student;
create table student (
sno char(9) not null,
sname char(8) not null,
ssex char(2) not null default '��',
sage tinyint(2),
sdept char(2)
);

insert into student values('200215121','����','��',20,'CS');
insert into student values('200215122','����','Ů',19,'CS');
insert into student values('200215123','����','Ů',18,'MA');
insert into student values('200215125','����','��',19,'IS');
