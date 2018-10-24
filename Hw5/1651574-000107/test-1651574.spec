Summary: SJHomework-RPM
Name: test-1651574
Version: 1.0.0
Release: 1%{?dist}

License:        GPL
URL:            https://github.com/Harrypotterrrr     
BuildRoot: /var/tmp/%{name}-buildroot               # 临时目录


%description
The project is made by jiahaolin.
I wanna be one girl's daemon, to protect and to love, to hold and to share, but who on earth could tell me how it could be..

%prep       # 编译预处理
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 工程编译预处理阶段 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%build      # 编译阶段
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 工程编译阶段 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
make


%install    # 安装阶段
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m RPM安装阶段 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
make install DESTDIR=%{buildroot}

%clean      # 清理操作
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 清理阶段 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
rm -rf %{buildroot}


%pre
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 准备安装 test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%post
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 完成安装 test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%preun
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 准备卸载 test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%postun
rm -fv ~/rpmbuild/BUILD/*
rm -fv ~/rpmbuild/RPMS/*.rpm
rm -fv ~/rpmbuild/SRPMS/*.rpm
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m 完成卸载 test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

# rpm -ivh 安装时的文件指定目录
%files
%defattr(-,root,root,-)
# defattr(<文件权限>, <用户>, <用户组>, <目录权限>) - 为默认
/etc/1651574.conf
/usr/1651574/1651574.dat
/usr/lib/systemd/system/test-1651574.service
/usr/lib64/lib1651574.so 
/usr/sbin/test-1651574

%changelog
* Wed Oct 24 2018 Jiahaolin <jiahaolin19971119@126.com> - 1.0
- Update to 1.0