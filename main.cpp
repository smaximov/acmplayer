#include <iostream>
#include <string>
#include <libfalltergeist.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <cassert>

namespace fall = libfalltergeist;

void usage(std::string programName)
{
    std::cerr << "Usage: " << std::endl;
    std::cerr << programName << "ACM_PATH" << std::endl;
    exit(EXIT_FAILURE);
}

class AcmPlayer
{
private:
    fall::DatFile _datFile;
    Uint16 _audio_format;
    int _audio_channels;
    int _audio_bitrate;

    void _check_ext(std::string file, std::string ext)
    {
        auto valid = false;
        try
        {
            if (file.substr(file.find_last_of('.') + 1) == ext)
            {
                valid = true;
            }
        }
        catch (...)
        {
        }

        if (!valid)
        {
            std::cerr << "Sorry, `" << file << "' doesn't appear to be a valid ACM file" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

public:
    AcmPlayer(std::string dat)
            : _datFile(dat)
    {
        assert(SDL_Init(SDL_INIT_AUDIO) == 0);
        assert(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 8192) == 0);
        Mix_QuerySpec(&_audio_bitrate, &_audio_format, &_audio_channels);
    }

    ~AcmPlayer()
    {
        Mix_CloseAudio();
        SDL_Quit();
    }

    void play(std::string path)
    {
        _check_ext(path, "acm");

        auto acm = std::dynamic_pointer_cast<fall::AcmFileType>(_datFile.item(path));
        acm->init();

        auto bitrate = acm->bitrate();
        auto cnt = acm->samples();
        auto channels = acm->channels();

        short *memory = (short *)malloc(cnt * 2);
        auto cnt1 = acm->read_samples(memory, cnt)*2;

        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt, AUDIO_S16SYS, channels, bitrate, _audio_format, _audio_channels, _audio_bitrate);

        cvt.buf = (Uint8*)malloc(cnt1*cvt.len_mult);
        memcpy(cvt.buf, (char*)memory, cnt1);
        cvt.len = cnt1;
        SDL_ConvertAudio(&cvt);
        // free old buffer
        free(memory);

        // make SDL_mixer chunk
        Mix_Chunk *chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len*cvt.len_ratio);

        if (chunk == nullptr)
        {
            throw std::runtime_error("Error loading chunk");
        }

        auto channel = Mix_PlayChannel(-1, chunk, 0);
        if(channel == -1)
        {
            throw std::runtime_error("Unable to play ACM file");
        }

        while(Mix_Playing(channel) != 0); // Wail until finished
        Mix_FreeChunk(chunk);
    }
};

int main(int argc, char *argv[])
{
    static const std::string dat = std::string(getenv("HOME")) + "/.falltergeist/master.dat";

    if (argc != 2) usage(*argv);

    try
    {
        AcmPlayer player(dat);
        player.play(argv[1]);
    }
    catch (fall::Exception &e) // TODO: make Exception::message const
    {
        std::cerr << e.message() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
