#include <conio.h>
#include <iostream>

#include "siren.h"

// Basic siren testing
int main()
{
	// Header
	printf("Siren v1.0 %s (DTZxPorter)\n\n", ((sizeof(uintptr_t) == 8) ? "x64" : "x86"));

	// Initialize the library
	auto Result = Siren::Initialize();

	// Resume on success
	if (Result == SirenStatus::Success)
	{
		// Log success
		printf("o Siren::Initialize() Success\n");

		// Make a buffer of all 0x55 for testing
		auto Buffer = new uint8_t[0x4000];
		// Set it 
		std::memset(Buffer, 0x55, 0x4000);

		// Calculate maximum size
		auto MaximumSize = Siren::CompressBound(0x4000);
		// Buffer for test data
		auto CompressedBuffer = new uint8_t[MaximumSize];
		// Clear it
		std::memset(CompressedBuffer, 0, MaximumSize);

		// Compress the buffer
		auto ResultSize = Siren::Compress(Buffer, 0x4000, CompressedBuffer, MaximumSize, SirenFormat::Kraken, SirenCompressionLevel::Normal);

		// If we're > 0 success
		if (ResultSize > 0)
		{
			// Log
			printf("o Siren::Compress() Success [%db => %db]\n", 0x4000, ResultSize);

			// Clean up
			std::memset(Buffer, 0, 0x4000);

			// Attempt to decompress it
			auto ResultDecompress = Siren::Decompress(CompressedBuffer, ResultSize, Buffer, 0x4000);

			// Verify it's all 0x55
			bool Verified = true;

			// Check
			for (uint32_t i = 0; i < 0x4000; i++) { if (Buffer[i] != 0x55) { Verified = false; } }

			// If we're 0x4000 success
			if (ResultDecompress == 0x4000 && Verified)
			{
				// Log
				printf("o Siren::Decompress() Success [%db => %db]\n", ResultSize, ResultDecompress);
			}
			else
			{
				// Failed
				printf("o Siren::Decompress() Failed, invalid result size or verification check\n");
			}
		}
		else
		{
			// Failed
			printf("o Siren::Compress() Failed, invalid result size\n");
		}

		// Clean up
		delete[] Buffer;
		delete[] CompressedBuffer;
	}
	else
	{
		// Failed
		printf("o Siren::Initialize() Failed, error code %d\n", (uint32_t)Result);
	}

	// Clean up
	Siren::Shutdown();
	
	// Done
	printf("\nPress any key to continue...");
	// Pause and end
	_getch(); return 0;
}