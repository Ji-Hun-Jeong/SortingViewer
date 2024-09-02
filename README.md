# SortingViewer  
설치 
=============
1. git설치
2. git clone https://github.com/Microsoft/vcpkg.git
3. bat파일 실행 후 exe파일 실행, vcpkg 폴더에서 git cmd실행
4. vcpkg install imgui[win32-binding,dx11-binding]:x64-windows     
5. vcpkg install directxtk:x64-windows
6. vcpkg install stb:x64-windows

이용방법  
=============
> w a s d q e로 이동할 수 있다.  
> f키로 화면을 움직일지 고정할지 정할 수 있다.  
> gui로 설정한 세팅대로 space바를 누르면 요소들이 생성된다.  
> 각 숫자 버튼에 따라 다른 정렬기능이 수행된다.  
> 또한 정렬이 진행중인 경우 같은 숫자 버튼을 누르면 즉시 정렬이 완료된다.  
> 대체적으로 빨간 색 요소는 정렬이 될 자리를 나타내고 파란색과 초록색은 비교대상이며 마젠타색은 퀵소트에서만 쓰이는 partition을 나타낸다.  
> 여기서 힙소트는 힙에 넣는 과정이 있기 때문에 일부러 늦게 재생한다.  
![image](https://github.com/user-attachments/assets/82aa47b0-c329-46ad-8d64-193d68c86fb6)
