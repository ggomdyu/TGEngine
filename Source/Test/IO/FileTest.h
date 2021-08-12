/**
 * @file    FileTest.h
 * @author  ggomdyu
 * @since   08/02/2019
 */

#pragma once
#include <cassert>

#include "IO/Path.h"
#include "IO/File.h"
#include "Platform/Environment.h"
#include "Diagnostics/Debug.h"

#include "../Test.h"

namespace tgon
{

class FileTest :
    public Test
{
/**@section Method */
public:
    void Evaluate() override
    {
        CreateTemporaryFileToEvaluateTest();
        
        auto desktopPath = *Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
        auto srcImagePath = *Environment::GetFolderPath(Environment::SpecialFolder::Desktop) + "/qwexqwexqw.png";

        auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
        auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);

        assert(File::SetCreationTime(srcImagePath, dtl));
        assert(File::GetCreationTime(srcImagePath).value() == dtu);
        assert(File::SetCreationTime(srcImagePath, dtu));
        assert(File::GetCreationTime(srcImagePath).value() == dtu.AddHours(9.0));

        File::SetCreationTimeUtc(srcImagePath, dtu);
        assert(File::GetCreationTimeUtc(srcImagePath).value() == dtu);
        File::SetCreationTimeUtc(srcImagePath, dtl);
        assert(File::GetCreationTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));

        File::SetLastWriteTime(srcImagePath, dtl);
        DateTime c = File::GetLastWriteTime(srcImagePath).value();
        assert(c == dtu);
        File::SetLastWriteTime(srcImagePath, dtu);
        c = File::GetLastWriteTime(srcImagePath).value();
        assert(c == dtu.AddHours(9.0));

        File::SetLastAccessTimeUtc(srcImagePath, dtu);
        assert(File::GetLastAccessTimeUtc(srcImagePath).value() == dtu);
        File::SetLastAccessTimeUtc(srcImagePath, dtl);
        assert(File::GetLastAccessTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));

        File::SetLastWriteTime(srcImagePath, dtl);
        assert(File::GetLastWriteTime(srcImagePath).value() == dtu);
        File::SetLastWriteTime(srcImagePath, dtu);
        assert(File::GetLastWriteTime(srcImagePath).value() == dtu.AddHours(9.0));

        File::SetLastWriteTimeUtc(srcImagePath, dtu);
        assert(File::GetLastWriteTimeUtc(srcImagePath).value() == dtu);
        File::SetLastWriteTimeUtc(srcImagePath, dtl);
        assert(File::GetLastWriteTimeUtc(srcImagePath).value() == dtu.AddHours(-9.0));

        auto destImagePath = desktopPath + "/1_q43289n231nk.png";
        File::Move(srcImagePath, destImagePath);
        assert(File::Exists(destImagePath));
        assert(File::Exists(srcImagePath) == false);
        File::Move(destImagePath, srcImagePath);

        File::Copy(srcImagePath, destImagePath);
        assert(File::Exists(destImagePath));

        File::Delete(destImagePath);
        assert(File::Exists(destImagePath) == false);

        File::Delete(srcImagePath);
    }

private:
    void CreateTemporaryFileToEvaluateTest()
    {
        auto filePath = *Environment::GetFolderPath(Environment::SpecialFolder::Desktop) + "/qwexqwexqw.png";
        File::Delete(filePath);

        FileStream f3(filePath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::None);;
        for (int i = 0; i < 1024; ++i)
        {
            f3.WriteByte(std::byte(i));
        }
        f3.Close();
    }
};

} /* namespace tgon */
