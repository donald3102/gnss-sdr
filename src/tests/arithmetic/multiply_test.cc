/*!
 * \file multiply_test.cc
 * \brief  This file implements tests for the multiplication of long arrays.
 * \author Carlos Aviles, 2010. carlos.avilesr(at)googlemail.com
 *         Carles Fernandez-Prades, 2012. cfernandez(at)cttc.es
 *
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2014  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */


#include <complex>
#include <ctime>
#include <armadillo>
#include <volk/volk.h>

DEFINE_int32(size_multiply_test, 100000, "Size of the arrays used for multiply testing");


TEST(Multiply_Test, StandardCDoubleImplementation)
{
    double* input = new double[FLAGS_size_multiply_test];
    double* output = new double[FLAGS_size_multiply_test];
    memset(input, 0, sizeof(double) * FLAGS_size_multiply_test);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long int begin = tv.tv_sec * 1000000 + tv.tv_usec;

    for(int i = 0; i < FLAGS_size_multiply_test; i++)
        {
            output[i] = input[i] * input[i];
        }

    gettimeofday(&tv, NULL);
    long long int end = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Element-wise multiplication of " << FLAGS_size_multiply_test
              << " doubles finished in " << (end - begin)
              << " microseconds" << std::endl;
    ASSERT_LE(0, end - begin);
    double acc = 0;
    double expected = 0;
    for(int i = 0; i < FLAGS_size_multiply_test; i++)
        {
            acc += output[i];
        }
    ASSERT_EQ(expected, acc);
    delete input;
    delete output;
}


TEST(Multiply_Test, ArmadilloImplementation)
{
    arma::vec input(FLAGS_size_multiply_test, arma::fill::zeros);
    arma::vec output(FLAGS_size_multiply_test);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long int begin = tv.tv_sec * 1000000 + tv.tv_usec;

    output = input % input;

    gettimeofday(&tv, NULL);
    long long int end = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Element-wise multiplication of " << FLAGS_size_multiply_test
              << "-length double armadillo vectors finished in " << (end - begin)
              << " microseconds" << std::endl;
    ASSERT_LE(0, end - begin);
    ASSERT_EQ(0, arma::norm(output));
}



TEST(Multiply_Test, StandardCComplexImplementation)
{
    std::complex<float>* input = new std::complex<float>[FLAGS_size_multiply_test];
    std::complex<float>* output = new std::complex<float>[FLAGS_size_multiply_test];
    memset(input, 0, sizeof(std::complex<float>) * FLAGS_size_multiply_test);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long int begin = tv.tv_sec * 1000000 + tv.tv_usec;

    for(int i = 0; i < FLAGS_size_multiply_test; i++)
        {
            output[i] = input[i] * input[i];
        }

    gettimeofday(&tv, NULL);
    long long int end = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Element-wise multiplication of " << FLAGS_size_multiply_test
              << " complex<float> finished in " << (end - begin)
              << " microseconds" << std::endl;
    ASSERT_LE(0, end - begin);
    std::complex<float> expected(0,0);
    std::complex<float> result(0,0);
    for(int i = 0; i < FLAGS_size_multiply_test; i++)
         {
             result += output[i];
         }

    ASSERT_EQ(expected, result);
    delete input;
    delete output;
}



TEST(Multiply_Test, ArmadilloComplexImplementation)
{
    arma::cx_fvec input(FLAGS_size_multiply_test, arma::fill::zeros);
    arma::cx_fvec output(FLAGS_size_multiply_test);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long int begin = tv.tv_sec * 1000000 + tv.tv_usec;

    output = input % input;

    gettimeofday(&tv, NULL);
    long long int end = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Element-wise multiplication of " << FLAGS_size_multiply_test
              << "-length complex float Armadillo vectors finished in " << (end - begin)
              << " microseconds" << std::endl;
    ASSERT_LE(0, end - begin);
    ASSERT_EQ(0, arma::norm(output));
}




TEST(Multiply_Test, VolkComplexImplementation)
{
    std::complex<float>* input = new std::complex<float>[FLAGS_size_multiply_test];
    std::complex<float>* output = new std::complex<float>[FLAGS_size_multiply_test];
    memset(input, 0, sizeof(std::complex<float>) * FLAGS_size_multiply_test);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long int begin = tv.tv_sec * 1000000 + tv.tv_usec;

    volk_32fc_x2_multiply_32fc(output, input, input, FLAGS_size_multiply_test);

    gettimeofday(&tv, NULL);
    long long int end = tv.tv_sec * 1000000 + tv.tv_usec;
    std::cout << "Element-wise multiplication of " << FLAGS_size_multiply_test
              << "-length complex float vector using VOLK finished in " << (end - begin)
              << " microseconds" << std::endl;
    ASSERT_LE(0, end - begin);

    float* mag = new float [FLAGS_size_multiply_test];
    volk_32fc_magnitude_32f(mag, output, FLAGS_size_multiply_test);
    float* result;
    volk_32f_accumulator_s32f(result, mag, FLAGS_size_multiply_test);
    // Comparing floating-point numbers is tricky.
    // Due to round-off errors, it is very unlikely that two floating-points will match exactly.
    // See http://code.google.com/p/googletest/wiki/AdvancedGuide#Floating-Point_Comparison
    float expected = 0;
    ASSERT_FLOAT_EQ(expected, result[0]);
    delete input;
    delete output;
    delete mag;
}
