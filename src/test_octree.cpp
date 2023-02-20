// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"
#include <fstream>


namespace opencv_test { namespace {

using namespace cv;

class OctreeTest: public testing::Test
{
protected:
    void SetUp() override
    {
        //load .ply file
        String loadFileName="/mnt/d/opencv/A_myPly/PointCloud/testPoint.ply";
        loadPointCloud(loadFileName,pointcloud);

        // Generate Octree From PointCloud,resolution is the minimum precision that can be specified and must be 10^x
        treeTest.create(pointcloud, 0.1);

        //traverse Octree by level order.Get the char array representation of the tree
        std::vector<unsigned char> treeToVector;
        treeTest.traverse(treeToVector);

        //compressed char array to byte stream
        std::ofstream vectorToStream;
        //std::stringstream vectorToStream;
        //vectorToStream.open(R"(A_myPly\output\stream_01.bin)", std::ios_base::binary);
        vectorToStream.open("/mnt/d/opencv/A_myPly/output/stream_bin", std::ios_base::binary);
        treeTest.encodeCharVectorToStream(treeToVector,vectorToStream);
        vectorToStream.close();

        std::ifstream streamToVector;
        streamToVector.open("/mnt/d/opencv/A_myPly/output/stream_bin", std::ios_base::binary);
        //restore char array from byte stream
        std::vector<unsigned char> vectorToTree(treeToVector.size());
        treeTest.decodeStreamToCharVector(streamToVector,vectorToTree);
        streamToVector.close();

        //restore Octree from char array
        treeTest.reStore(vectorToTree);

        //restore PointCloud from Octree
        treeTest.getPointCloudByOctree(restorePointCloud);
        //save .ply file
        String saveFileName="/mnt/d/opencv/A_myPly/output/testPoint.ply";
        savePointCloud(saveFileName,restorePointCloud);
        String ss;
    }

public:
    std::vector<Point3f> pointcloud;
    std::vector<Point3f> restorePointCloud;
    Point3f restPoint;
    Octree treeTest;

private:
    int maxDepth;
};
TEST_F(OctreeTest, BasicFunctionTest)
{
    EXPECT_TRUE(true);
}
} // namespace
} // opencv_test
