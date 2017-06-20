#
# IBM Service Manifest Library .spec file
#
%define __spec_install_post %{nil}
%define __os_install_post %{_dbpath}/brp-compress
%define debug_package %{nil}

Summary: IBM Service Manifest runtime library
Name: libibmmanifest
Version: 1.0
Release: 1
Group: Misc
License: Restricted
Source: libibmmanifest.tar.gz
%description
IBM Internal Service component manifest run time library

%prep
%setup -n %{name}

%build
#do nothinig

%install
install -m0755 -d %{buildroot}/usr/lib
install -m0755 -d %{buildroot}/usr/share/doc/libibmmanifest1
install -m0755 -t %{buildroot}/usr/lib usr/lib/*
install -m0644 -t %{buildroot}/usr/share/doc/libibmmanifest1 usr/share/doc/libibmmanifest1/*

%files
/usr/lib/libibmmanifest.so.1.0.0
/usr/share/doc/libibmmanifest1/changelog

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%clean

%changelog

