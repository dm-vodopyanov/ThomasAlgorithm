import os
import sys
import platform
import datetime

def help():
    print("Usage:")
    print("    <dimension>  <min element in matrix>  <max element in matrix>  <version_###>  [<version_###> ...]")

if __name__ == "__main__":
    curr_time = datetime.datetime.now().isoformat()
    curr_time = curr_time.replace(":",".")
    os.system("mkdir " + curr_time)

    delim = " & "
    if len(sys.argv) == 1:
        help()
    elif (len(sys.argv) > 1) and (len(sys.argv) < 4):
        print("ERROR: Should be at least 5 arguments")
        help()
    elif (len(sys.argv) >= 4):
        ipsxe_path = ""
        if platform.system() == 'Windows':
            if (os.path.exists("C:\\Program Files (x86)\\Intel\\Composer XE 2015\\bin")):
                ipsxe_path = "\"C:\Program Files (x86)\\Intel\\Composer XE 2015\\bin\\"
            elif (os.path.exists("C:\\Program Files (x86)\\IntelSWTools\\compilers_and_libraries\\windows\\bin")):
                ipsxe_path = "\"C:\Program Files (x86)\\IntelSWTools\\compilers_and_libraries\windows\\bin\\"
            icc_env = ipsxe_path + "compilervars.bat\"" + " " + "intel64"
        elif platform.system() == 'Linux':
            icc_env = "source /opt/intel/compilers_and_libraries/linux/bin/compilervars.sh intel64"

        print("="*79)
        print("Compiling MatrixGeneration project...")
        print("="*79)

        if platform.system() == 'Windows':
            os.system(icc_env + " & " + "icl.exe ..\src\main_gen_matrix.cpp /O2 /D NDEBUG")
        elif platform.system() == 'Linux':
            os.system(icc_env + " ; " + "icc -O2 -DNDEBUG -m64 -openmp -o main_gen_matrix ../src/main_gen_matrix.cpp")

        if platform.system() == 'Windows':
            os.system("copy main_gen_matrix.exe " + curr_time)
        elif platform.system() == 'Linux':
            os.system("cp main_gen_matrix " + curr_time)
            os.system("rm main_gen_matrix")

        print("="*79)
        print("Running MatrixGeneration project...")
        print("="*79)

        if platform.system() == 'Windows':
            os.system("cd " + curr_time + "& main_gen_matrix.exe " + sys.argv[1] + " " + sys.argv[2] + " " + sys.argv[3])
        elif platform.system() == 'Linux':
            os.system("cd " + curr_time + "; ./main_gen_matrix " + sys.argv[1] + " " + sys.argv[2] + " " + sys.argv[3])

        i = 4
        while (i < len(sys.argv)):
            print("="*79)
            print("Compiling Version_" + sys.argv[i] + " project...")
            print("="*79)
            if platform.system() == 'Windows':
                os.system(icc_env + delim + "icl.exe ..\src\main_" + sys.argv[i] + ".cpp /O2 /D NDEBUG /Qopenmp")
                os.system("copy main_" + sys.argv[i] + ".exe " + curr_time)
                os.system("del main_" + sys.argv[i] + ".exe")
            elif platform.system() == 'Linux':
                os.system(icc_env + " ; " + "icc ../src/main_" + sys.argv[i] + ".cpp -O2 -DNDEBUG -m64 -openmp -o main_" + sys.argv[i])
                os.system("cp main_" + sys.argv[i] + " " + curr_time)
                os.system("rm main_" + sys.argv[i] + "")

            print("="*79)
            print("Running Version_" + sys.argv[i] + " project...")
            print("="*79)
            if platform.system() == 'Windows':
                os.system("cd " + curr_time + "& main_" + sys.argv[i] + ".exe " + "inputfile outputfile_" + sys.argv[i] + " timefile_" + sys.argv[i])
            elif platform.system() == 'Linux':
                os.system("cd " + curr_time + " ; ./main_" + sys.argv[i] + " " + "inputfile outputfile_" + sys.argv[i] + " timefile_" + sys.argv[i])
            i = i + 1

        print("="*79)
        print("Compiling and running completed.")
        print("="*79)

        if platform.system() == 'Windows':
            os.system("del *.obj")
            os.system("del *.exe")

        print("")

        log = open(curr_time + '/log.txt', 'w')

        print("-"*79)
        print("Matrix dimention: " + sys.argv[1])
        log.write("Matrix dimention: " + sys.argv[1] + "\n")
        log.write("Minimum exement in matrix: " + sys.argv[2] + "\n")
        log.write("Maximum exement in matrix: " + sys.argv[3] + "\n")
        print("Execution times of implementations:")
        log.write("Execution times of implementations:" + "\n")

        i = 4
        while (i < len(sys.argv)):
            tf = open(curr_time + '/timefile_' + sys.argv[i], 'r')
            time = tf.read()
            s = time.find("\n")
            print("  version_" + sys.argv[i] + ": " + time[:s])
            log.write("  version_" + sys.argv[i] + ": " + time[:s] + "\n")
            tf.close()
            i = i + 1

        eps = 0.001

        of1 = open(curr_time + '/outputfile_' + sys.argv[4], 'r')
        if (len(sys.argv) >= 6):
            print("Files comparison:")
            log.write("Files comparison:\n")
            print("  " + sys.argv[4] + " vs " + sys.argv[5])
            log.write("  " + sys.argv[4] + " vs " + sys.argv[5] + "\n")
            of2 = open(curr_time + '\\outputfile_' + sys.argv[5], 'r')
            for line1 in of1:
                for line2 in of2:
                    if abs(float(line1)-float(line2)) > eps:
                        print("    ERROR: " + line1[:line1.find("\n")] + " " + line2[:line2.find("\n")])
                        log.write("    ERROR: " + line1[:line1.find("\n")] + " " + line2[:line2.find("\n")] + "\n")
                    break
        if (len(sys.argv) >= 7):
            print("  " + sys.argv[4] + " vs " + sys.argv[6])
            log.write("  " + sys.argv[4] + " vs " + sys.argv[6] + "\n")
            of3 = open(curr_time + '/outputfile_' + sys.argv[6], 'r')
            for line1 in of1:
                for line3 in of3:
                    if abs(float(line1)-float(line3)) > eps:
                        print("    ERROR: " + line1[:line1.find("\n")] + " " + line3[:line3.find("\n")])
                        log.write("    ERROR: " + line1[:line1.find("\n")] + " " + line3[:line3.find("\n")] + "\n")
                    break
        if (len(sys.argv) == 8):
            print("  " + sys.argv[4] + " vs " + sys.argv[7])
            log.write("  " + sys.argv[4] + " vs " + sys.argv[7] + "\n")
            of4 = open(curr_time + '/outputfile_' + sys.argv[7], 'r')
            for line1 in of1:
                for line4 in of4:
                    if abs(float(line1)-float(line4)) > eps:
                        print("    ERROR: " + line1[:line1.find("\n")] + " " + line4[:line4.find("\n")])
                        log.write("    ERROR: " + line1[:line1.find("\n")] + " " + line4[:line4.find("\n")] + "\n")
                    break

        print("-"*79)
        log.close()
