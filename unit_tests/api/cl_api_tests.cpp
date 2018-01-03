/*
 * Copyright (c) 2017, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "cl_api_tests.h"
#include "runtime/command_queue/command_queue.h"
#include "runtime/helpers/options.h"
#include "unit_tests/mocks/mock_context.h"
#include "unit_tests/mocks/mock_kernel.h"

namespace OCLRT {

api_fixture::api_fixture()
    : retVal(CL_SUCCESS), retSize(0), pCommandQueue(nullptr),
      pContext(nullptr), pKernel(nullptr), pProgram(nullptr) {
}

void api_fixture::SetUp() {
    MemoryManagementFixture::SetUp();
    PlatformFixture::SetUp(numPlatformDevices, platformDevices);

    ASSERT_EQ(retVal, CL_SUCCESS);
    auto pDevice = pPlatform->getDevice(0);
    ASSERT_NE(nullptr, pDevice);

    cl_device_id clDevice = pDevice;
    pContext = Context::create<MockContext>(nullptr, DeviceVector(&clDevice, 1), nullptr, nullptr, retVal);

    pCommandQueue = new CommandQueue(pContext, pDevice, 0);

    pProgram = new MockProgram(pContext);

    pKernel = new MockKernel(pProgram, *pProgram->MockProgram::getKernelInfo(), *pDevice);
    ASSERT_NE(nullptr, pKernel);
    BuiltInFixture::SetUp(pDevice);
}

void api_fixture::TearDown() {
    delete pKernel;
    delete pCommandQueue;
    pContext->release();
    pProgram->release();

    BuiltInFixture::TearDown();
    PlatformFixture::TearDown();
    MemoryManagementFixture::TearDown();
}
} // namespace OCLRT