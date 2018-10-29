<!-- <meta charset="utf-8" lang="en"><style class="fallback">body{visibility:hidden;}</style> -->

**Socket编程 TCP阻塞方式**
    1651574 贾昊霖

# 克隆与添加网卡多地址

## 再往网卡上设置多地址

首先仿照/etc/sysconfig/network-scripts/ifcfg-ens32增加一文件根据网络虚拟接口的名字进行命名，命名为ifcfg-ens32:0

修改文件如下

[](./pic/0-1.png)

观察网卡配置

[](./pic/0-2.png)


在另一虚拟机下成功ping通

[](./pic/0-3.png)

# Socket
[学习网站](https://www.cnblogs.com/jiangzhaowei/p/8261174.html)







<!-- 
<!-Markdeep: --><style class="fallback">body{visibility:hidden}</style><script src="../lib/markdeep.min.js"></script>
<script src="../lib/jquery-3.3.1.min.js"></script>

<script type="text/javascript">
    $(document).ready(function(){setTimeout(function(){$(".image a").removeAttr("href")}, 0)});
</script>

<style>
    /* .md h2::before{
        content: counter(h2, lower-alpha)
    }

    .md h2.notinc::before{
        counter-increment: none;
        content: none
    }

    div.notinc1 h1::before{
        counter-increment: none;
        content: "2*"
    }

    .md h1.notinc::before{
        counter-increment: none;
    } */

    .md .image {
        width: 80%;
    }

    .md img {
        border: 1.3px rgb(0, 0, 0) solid;
    }

    .md div.imagecaption {
        text-align: center;
    }

    /* .level2 .tocnumber {
        display: none
    } */

    .md code {
        background-color: rgba(255, 249, 158, 0.788);
        color: rgb(145, 52, 40);
    }

    .md pre code {
        background: none;
        /* background-color: rgb(250, 238, 224); */
        /* border: 1.0px rgb(0, 0, 0) solid; */
    }

    body {
        font-family: 等线,Palatino,Georgia,"Times New Roman",serif;
    }
</style> -->
