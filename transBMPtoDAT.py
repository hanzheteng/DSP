# transBMPtoDAT.py
# 将bmp文件转换为dat文件，用于Visual Dsp++程序进行图像处理
# 本程序适用于宽度和高度都在65536像素以内的24位真彩色存储的bmp图像
# 请将需要转换的bmp图片都放在一个文件夹下，并将主程序中的path路径设置成这个文件夹路径

def readfile(pathin):
    with open(pathin, 'rb') as fin:
        file = fin.read()
    return file

def writehead(pathout, head):
    with open(pathout, 'wb') as fout:
        for ch in head:
            c = hex(ch) + '\r\n,'
            fout.write(c.encode('iso-8859-1'))

def writeline(pathout, line, width):
    with open(pathout, 'ab') as fout:
        for i in range(width * 3):
            ch = hex(line[i]) + '\r\n,'
            fout.write(ch.encode('iso-8859-1'))

def transOneFile(pathin, pathout):
    file = readfile(pathin)
    head = file[0:54]  #前54个字节是bmp头文件
    body = file[54:]  #后续是按BGR顺序存储的3通道8bit图像数据
    width = head[19]*256 + head[18]  #文件宽度存储在第19-22个字节里
    height = head[23]*256 + head[22]  #文件高度存储在第23-26个字节里
    blank = -(width * 3) % 4  #计算每行末尾需要用0补齐的字节数
    rowlen = width * 3 + blank  #实际每行占用空间（字节数）

    if len(file) != 54 + rowlen * abs(height):
        print("file size ERROR\n")
        return False
    writehead(pathout, head)
    if height > 0:
        for row in range(height):
            writeline(pathout, body[(height-row-1)*rowlen:(height-row)*rowlen], width)
    else:
        for row in range(abs(height)):
            writeline(pathout, body[row*rowlen:(row+1)*rowlen], width)

if __name__ == '__main__':
    import os
    path = "d://dsp"  #请将路径path修改成bmp文件所在的文件夹
    listdir = os.listdir(path)
    for filename in listdir:
        if filename.split('.')[1] == 'bmp':
            pathin = path + '/' + filename
        else:
            continue
        pathout = path + '/' + filename.split('.')[0] + '.dat'
        transOneFile(pathin, pathout)
    print("Finish\n")