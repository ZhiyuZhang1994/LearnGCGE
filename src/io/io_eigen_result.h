/**
 * @brief 特征值和特征向量写入txt文件，从txt文件读入已有结果
 * @author mading
 * @date 2025-02-21
 */

#ifndef _IO_EIGEN_RESULT_H_
#define _IO_EIGEN_RESULT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#define PRECISION 6     // 写入txt文件时，保存double数据的小数位数
#define PROTOCOL_LINE 9 // 特征值和特征向量文件协议描述行数

class IoEigenResult {
public:
    std::vector<double> eigenvalue_;                // 读取时，保存特征值
    std::vector<std::vector<double>> eigenvector_;  // 读取时，保存特征向量

private:
    int numEigenvalue_ = 0;  // 特征值个数
    int dimension_ = 0;      // 特征向量维度

public:
    /**
     * @brief 特征值和特征向量结果写入txt文件
     *
     * @param eigenvalue 特征值
     * @param eigenvector 特征向量
     * @param fileName 保存文件的名称
     * @return int 错误码
     */
    int saveEigenResult(const std::vector<double>& eigenvalue, const std::vector<std::vector<double>>& eigenvector, std::string fileName = "eigenResult.txt");

    /**
     * @brief 读取特征值和特征向量结果文件
     *
     * @param fileName 结果文件名称
     * @return int 错误码
     */
    int readEigenFile(std::string fileName = "eigenResult.txt");

private:
    /**
     * @brief 将一个vector写入txt文件
     *
     * @param outFile 输出流ofstream
     * @param oneVector 一个vector数据
     * @return int 错误码
     */
    int saveOneVector(std::ofstream& outFile, const std::vector<double>& oneVector);

    /**
     * @brief 读入特征值数据
     *
     * @param eigenvalueStr 特征值所指行的字符串
     * @return int 错误码
     */
    int readEigenvalue(std::string& eigenvalueStr);

    /**
     * @brief 读入特征向量数据
     *
     * @param eigenVectorStr 某一行特征向量所指的字符串
     * @return int 错误码
     */
    int readEigenVector(std::string& eigenVectorStr);
};

#endif
