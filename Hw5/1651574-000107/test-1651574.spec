Summary: SJHomework-RPM
Name: test-1651574
Version: 1.0.0
Release: 1%{?dist}

License:        GPL
URL:            https://github.com/Harrypotterrrr     
BuildRoot: /var/tmp/%{name}-buildroot               # ��ʱĿ¼


%description
The project is made by jiahaolin.
I wanna be one girl's daemon, to protect and to love, to hold and to share, but who on earth could tell me how it could be..

%prep       # ����Ԥ����
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ���̱���Ԥ����׶� \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%build      # ����׶�
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ���̱���׶� \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
make


%install    # ��װ�׶�
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m RPM��װ�׶� \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
make install DESTDIR=%{buildroot}

%clean      # �������
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ����׶� \033[0m"
echo -e "\e[1;33m ********************** \033[0m"
rm -rf %{buildroot}


%pre
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ׼����װ test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%post
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ��ɰ�װ test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%preun
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ׼��ж�� test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

%postun
rm -fv ~/rpmbuild/BUILD/*
rm -fv ~/rpmbuild/RPMS/*.rpm
rm -fv ~/rpmbuild/SRPMS/*.rpm
echo -e "\e[1;33m ********************** \033[0m"
echo -e "\e[1;33m ���ж�� test-1651574 \033[0m"
echo -e "\e[1;33m ********************** \033[0m"

# rpm -ivh ��װʱ���ļ�ָ��Ŀ¼
%files
%defattr(-,root,root,-)
# defattr(<�ļ�Ȩ��>, <�û�>, <�û���>, <Ŀ¼Ȩ��>) - ΪĬ��
/etc/1651574.conf
/usr/1651574/1651574.dat
/usr/lib/systemd/system/test-1651574.service
/usr/lib64/lib1651574.so 
/usr/sbin/test-1651574

%changelog
* Wed Oct 24 2018 Jiahaolin <jiahaolin19971119@126.com> - 1.0
- Update to 1.0