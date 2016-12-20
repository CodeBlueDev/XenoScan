#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <functional>

#include "ScannerTypes.h"

#include "ScannerTarget.h"
#include "ScanVariant.h"
#include "ScanResult.h"
#include "ScanState.h"

class Scanner
{
public:
	typedef uint32_t ScanInferType;
	enum _ScanInferType : ScanInferType
	{
		SCAN_INFER_TYPE_ALL_TYPES,
		SCAN_INFER_TYPE_STRING_TYPES,
		SCAN_INFER_TYPE_NUMERIC_TYPES,
		SCAN_INFER_TYPE_EXACT,

		SCAN_INFER_TYPE_END = SCAN_INFER_TYPE_NUMERIC_TYPES
	};

	enum _CompareTypeFlags : CompareTypeFlags
	{
		SCAN_COMPARE_BEGIN = 1,

		SCAN_COMPARE_EQUALS = SCAN_COMPARE_BEGIN,
		SCAN_COMPARE_GREATER_THAN = 2,
		SCAN_COMPARE_LESS_THAN = 4,
		SCAN_COMPARE_GREATER_THAN_OR_EQUALS = 3,
		SCAN_COMPARE_LESS_THAN_OR_EQUALS = 5,

		SCAN_COMPARE_END = SCAN_COMPARE_LESS_THAN_OR_EQUALS
	};

	ScanStateShPtr scanState;

	Scanner();
	~Scanner();

	void startNewScan();
	void runScan(const ScannerTargetShPtr &target, const ScanVariant &needle, const CompareTypeFlags &comp, const ScanInferType &type);
	void runDataStructureScan(const ScannerTargetShPtr &target);
	
private:
	struct typeRange
	{
		ScanVariant::ScanVariantType low, high;
		typeRange(ScanVariant::ScanVariantType low, ScanVariant::ScanVariantType high) : low(low), high(high) {}
		typeRange() : low(ScanVariant::SCAN_VARIANT_ALLTYPES_BEGIN), high(ScanVariant::SCAN_VARIANT_ALLTYPES_BEGIN) {}
	};
	typeRange typeRangeMap[SCAN_INFER_TYPE_END + 1];

	MemoryInformationCollection getScannableBlocks(const ScannerTargetShPtr &target) const;

	typedef std::function<void(const MemoryAddress &baseAddress, const uint8_t* chunk, const size_t &chunkSize)> blockIterationCallback;
	void iterateOverBlocks(const ScannerTargetShPtr &target, const MemoryInformationCollection &blocks, blockIterationCallback callback) const;
	void calculateBoundsOfBlocks(const ScannerTargetShPtr &target, const MemoryInformationCollection &blocks, MemoryAddress &lower, MemoryAddress &upper) const;
	inline bool isValidPointer(const MemoryAddress &lower, const MemoryAddress &upper, const MemoryAddress &address) const
	{
		return (address >= lower && address <= upper);
	}

	void doScan(const ScannerTargetShPtr &target, const ScanResultCollection &needles, const CompareTypeFlags &compType);
	void doReScan(const ScannerTargetShPtr &target, const ScanResultCollection &needles, const CompareTypeFlags &compType);

	void doDataStructureScan(const ScannerTargetShPtr &target);
};
typedef std::shared_ptr<Scanner> ScannerShPtr;